#include "ZetaAssistantSystem.h"
#include "spacecraft/SpacecraftState.h"
#include <QMediaDevices>
#include <QAudioFormat>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QStandardPaths>
#include <QUrl>
#include <QRegularExpression>
#include <QHash>
#include <QStringList>
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <utility>

// Change this authorization PIN whenever you want (digits or spoken words will match).
static const QString AUTH_PIN = QStringLiteral("1234");

ZetaAssistantSystem::ZetaAssistantSystem(QObject *parent)
    : QObject(parent)
{
    m_inputWavPath = QDir::temp().filePath("zeta_stt_input.wav");
    m_outputWavPath = QDir::temp().filePath("zeta_tts_output.wav");

    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_audioOutput->setVolume(1.0f);
    m_mediaPlayer->setAudioOutput(m_audioOutput);

    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        bool speaking = (state == QMediaPlayer::PlayingState);
        if (m_isSpeaking != speaking) {
            m_isSpeaking = speaking;
            emit isSpeakingChanged();
        }
        if (!speaking && !m_isListening) {
            m_audioStatus = "STANDBY";
            emit audioStatusChanged();
        }
    });

    m_piperProcess = new QProcess(this);
    connect(m_piperProcess, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit && exitCode == 0) {
            m_mediaPlayer->setSource(QUrl());
            m_mediaPlayer->setSource(QUrl::fromLocalFile(m_outputWavPath));
            m_mediaPlayer->play();
            m_audioStatus = "TRANSMITTING VOICE...";
            emit audioStatusChanged();
        } else {
            qWarning() << "Piper TTS failed:" << m_piperProcess->readAllStandardError();
            m_audioStatus = "TTS SYNTHESIS ERROR";
            emit audioStatusChanged();
        }
    });

    m_whisperProcess = new QProcess(this);
    connect(m_whisperProcess, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit && exitCode == 0) {
            QString transcript = QString::fromUtf8(m_whisperProcess->readAllStandardOutput()).trimmed();
            transcript.remove(QRegularExpression("\\[.*?\\]|\\(.*?\\)"));
            transcript = transcript.simplified();

            if (transcript.isEmpty()) {
                m_lastHeardText = "[No intelligible speech detected]";
                emit lastHeardTextChanged();
                m_lastResponse = "Unable to parse voice transmission, Captain. Please repeat.";
                emit lastResponseChanged();
                speakResponse(m_lastResponse);
            } else {
                m_lastHeardText = transcript;
                emit lastHeardTextChanged();
                processCommand(transcript);
            }
        } else {
            qWarning() << "Whisper STT failed:" << m_whisperProcess->readAllStandardError();
            m_audioStatus = "STT TRANSCRIPTION ERROR";
            emit audioStatusChanged();
        }
    });
}

ZetaAssistantSystem::~ZetaAssistantSystem()
{
    if (m_audioSource)
    {
        m_audioSource->stop();
        delete m_audioSource;
    }
    for (QProcess *proc : {m_whisperProcess, m_piperProcess})
    {
        if (proc && proc->state() != QProcess::NotRunning)
        {
            proc->kill();
            proc->waitForFinished(300);
        }
    }
}

void ZetaAssistantSystem::setSpacecraftState(SpacecraftState *state)
{
    m_spacecraftState = state;
}

QString ZetaAssistantSystem::lastResponse() const { return m_lastResponse; }
QString ZetaAssistantSystem::lastHeardText() const { return m_lastHeardText; }
bool ZetaAssistantSystem::isListening() const { return m_isListening; }
bool ZetaAssistantSystem::isSpeaking() const { return m_isSpeaking; }
bool ZetaAssistantSystem::awaitingAuthorization() const { return m_awaitingAuthorization; }
QString ZetaAssistantSystem::audioStatus() const { return m_audioStatus; }

QString ZetaAssistantSystem::resolveAssetPath(const QString &relativePath) const
{
    QStringList searchBases = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QCoreApplication::applicationDirPath() + "/..",
        QCoreApplication::applicationDirPath() + "/../.."};

    for (const QString &base : searchBases)
    {
        QFileInfo candidate(QDir(base).filePath(relativePath));
        if (candidate.exists())
        {
            return candidate.absoluteFilePath();
        }
    }
    return QDir::current().absoluteFilePath(relativePath);
}

bool ZetaAssistantSystem::writeWavFile(const QString &filePath, const QByteArray &pcmData, int sampleRate) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream out(&file);
    out.setByteOrder(QDataStream::LittleEndian);

    const quint16 numChannels = 1;
    const quint16 bitsPerSample = 16;
    const quint32 byteRate = sampleRate * numChannels * (bitsPerSample / 8);
    const quint16 blockAlign = numChannels * (bitsPerSample / 8);
    const quint32 dataSize = static_cast<quint32>(pcmData.size());
    const quint32 chunkSize = 36 + dataSize;

    file.write("RIFF", 4);
    out << chunkSize;
    file.write("WAVE", 4);

    file.write("fmt ", 4);
    out << quint32(16);
    out << quint16(1);
    out << numChannels;
    out << quint32(sampleRate);
    out << byteRate;
    out << blockAlign;
    out << bitsPerSample;

    file.write("data", 4);
    out << dataSize;
    file.write(pcmData);

    file.close();
    return true;
}

void ZetaAssistantSystem::speakResponse(const QString &text)
{
    if (text.trimmed().isEmpty())
        return;

    if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState)
    {
        m_mediaPlayer->stop();
    }
    if (m_piperProcess->state() != QProcess::NotRunning)
    {
        m_piperProcess->kill();
        m_piperProcess->waitForFinished(200);
    }

    QString modelPath = resolveAssetPath("assets/voices/en_US-libritts_r-medium.onnx");
    if (!QFileInfo::exists(modelPath))
    {
        qWarning() << "Piper model missing at:" << modelPath;
        m_audioStatus = "MISSING ONNX MODEL";
        emit audioStatusChanged();
        return;
    }

    m_audioStatus = "SYNTHESIZING VOICE...";
    emit audioStatusChanged();

    QStringList args = {
        "--model", modelPath,
        "--speaker", QString::number(ZETA_SPEAKER_ID),
        "--output_file", m_outputWavPath};

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    env.insert("PATH", homeDir + "/.local/bin:" + env.value("PATH"));
    m_piperProcess->setProcessEnvironment(env);

    m_piperProcess->start("piper", args);
    if (m_piperProcess->waitForStarted(1000))
    {
        m_piperProcess->write(text.toUtf8() + "\n");
        m_piperProcess->closeWriteChannel();
    }
    else
    {
        qWarning() << "Could not launch piper executable.";
        m_audioStatus = "PIPER EXECUTABLE NOT FOUND";
        emit audioStatusChanged();
    }
}

void ZetaAssistantSystem::startListening()
{
    if (m_isListening) return;

    if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        m_mediaPlayer->stop();
    }

    m_recordedAudioData.clear();
    m_audioBuffer.setBuffer(&m_recordedAudioData);
    m_audioBuffer.open(QIODevice::WriteOnly);

    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    auto inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
        m_audioStatus = "ERROR: NO MICROPHONE DETECTED";
        emit audioStatusChanged();
        return;
    }

    qDebug() << "Activating audio input device:" << inputDevice.description();

    m_audioSource = new QAudioSource(inputDevice, format, this);
    m_audioSource->start(&m_audioBuffer);

    m_isListening = true;
    m_audioStatus = "RECORDING VOICE BUFFER...";
    emit isListeningChanged();
    emit audioStatusChanged();
}

void ZetaAssistantSystem::stopListening()
{
    if (!m_isListening)
        return;

    if (m_audioSource)
    {
        m_audioSource->stop();
        delete m_audioSource;
        m_audioSource = nullptr;
    }
    m_audioBuffer.close();

    m_isListening = false;
    m_audioStatus = "TRANSCRIBING SPEECH...";
    emit isListeningChanged();
    emit audioStatusChanged();

    processAudioBuffer(m_recordedAudioData);
}

void ZetaAssistantSystem::processAudioBuffer(const QByteArray &pcmData)
{
    if (pcmData.size() < 6400)
    {
        m_lastResponse = "Audio input too short. Hold the comms key while speaking.";
        m_audioStatus = "STANDBY";
        emit lastResponseChanged();
        emit audioStatusChanged();
        speakResponse(m_lastResponse);
        return;
    }

    if (!writeWavFile(m_inputWavPath, pcmData, 16000))
    {
        m_audioStatus = "WAV WRITE ERROR";
        emit audioStatusChanged();
        return;
    }

    QString whisperBin = resolveAssetPath("assets/stt/whisper-cli");
    QString whisperModel = resolveAssetPath("assets/stt/ggml-tiny.en.bin");

    if (!QFileInfo::exists(whisperBin) || !QFileInfo::exists(whisperModel))
    {
        qWarning() << "Whisper binary or model missing:" << whisperBin << whisperModel;
        m_audioStatus = "MISSING WHISPER STT ASSETS";
        emit audioStatusChanged();
        return;
    }

    if (m_whisperProcess->state() != QProcess::NotRunning)
    {
        m_whisperProcess->kill();
        m_whisperProcess->waitForFinished(200);
    }

    QStringList args = {
        "-m", whisperModel,
        "-f", m_inputWavPath,
        "-nt",
        "-np"};

    m_whisperProcess->start(whisperBin, args);
}

void ZetaAssistantSystem::requestAuthorization(const QString &prompt, std::function<QString()> action)
{
    m_pendingAction = std::move(action);
    m_pendingTimer.start();
    m_lastResponse = prompt + " State authorization PIN to execute, or say cancel.";

    if (!m_awaitingAuthorization) {
        m_awaitingAuthorization = true;
        emit awaitingAuthorizationChanged();
    }
}

void ZetaAssistantSystem::clearPendingAuthorization()
{
    m_pendingAction = nullptr;
    if (m_awaitingAuthorization) {
        m_awaitingAuthorization = false;
        emit awaitingAuthorizationChanged();
    }
}

void ZetaAssistantSystem::processCommand(const QString &command)
{
    QString cmd = command.trimmed().toLower();
    // Drop punctuation but keep decimal points ("93.5"), split hyphenated words ("forty-two")
    cmd.remove(QRegularExpression("[,!?;:]|\\.(?!\\d)"));
    cmd.replace(QRegularExpression("(?<=[a-z])-(?=[a-z])"), " ");
    cmd = cmd.simplified();

    if (!m_spacecraftState)
    {
        m_lastResponse = "Critical fault: Flight control backplane is offline. I am flying blind here, Captain.";
        emit lastResponseChanged();
        speakResponse(m_lastResponse);
        return;
    }

    auto has = [&cmd](const QString &pattern) {
        return cmd.contains(QRegularExpression("\\b(?:" + pattern + ")\\b"));
    };

    auto extractPinDigits = [&cmd]() -> QString {
        QString digitsOnly;
        for (const QChar &ch : cmd) {
            if (ch.isDigit()) {
                digitsOnly.append(ch);
            }
        }
        if (!digitsOnly.isEmpty()) {
            return digitsOnly;
        }

        static const QHash<QString, QString> wordToDigit = {
            {"zero", "0"}, {"oh", "0"}, {"o", "0"},
            {"one", "1"}, {"won", "1"},
            {"two", "2"}, {"to", "2"}, {"too", "2"},
            {"three", "3"},
            {"four", "4"}, {"for", "4"},
            {"five", "5"},
            {"six", "6"},
            {"seven", "7"},
            {"eight", "8"}, {"ate", "8"},
            {"nine", "9"}
        };

        QString spokenDigits;
        const QStringList words = cmd.split(' ', Qt::SkipEmptyParts);
        for (const QString &w : words) {
            if (wordToDigit.contains(w)) {
                spokenDigits.append(wordToDigit.value(w));
            }
        }
        return spokenDigits;
    };

    // Handle pending PIN authorization first
    if (m_awaitingAuthorization) {
        if (m_pendingTimer.hasExpired(AUTH_TIMEOUT_MS)) {
            clearPendingAuthorization();
            m_lastResponse = "Authorization request timed out, Captain.";
            emit lastResponseChanged();
            speakResponse(m_lastResponse);
            return;
        }
        if (has("cancel|negative|no|nevermind")) {
            clearPendingAuthorization();
            m_lastResponse = "Pending command cancelled, Captain.";
            emit lastResponseChanged();
            speakResponse(m_lastResponse);
            return;
        }

        const QString expectedPin = AUTH_PIN.trimmed();
        const QString spokenPin = extractPinDigits();
        if (!expectedPin.isEmpty() && spokenPin == expectedPin) {
            auto action = std::move(m_pendingAction);
            clearPendingAuthorization();
            m_lastResponse = action ? action() : "No pending action found.";
            emit lastResponseChanged();
            speakResponse(m_lastResponse);
            return;
        }

        m_lastResponse = spokenPin.isEmpty()
            ? "Authorization required. State the PIN or say cancel."
            : "Invalid authorization PIN, Captain. State the valid PIN or say cancel.";
        emit lastResponseChanged();
        speakResponse(m_lastResponse);
        return;
    }

    auto extractNumber = [&cmd, &has](bool &found) -> float {
        found = true;

        QRegularExpressionMatch m = QRegularExpression("-?\\d+(?:\\.\\d+)?").match(cmd);
        if (m.hasMatch())
            return m.captured(0).toFloat();

        if (has("hundred|full|max|maximum")) return 100.0f;
        if (has("half")) return 50.0f;
        if (has("idle")) return 0.0f;

        static const QHash<QString, int> tens = {
            {"twenty", 20}, {"thirty", 30}, {"forty", 40}, {"fifty", 50},
            {"sixty", 60}, {"seventy", 70}, {"eighty", 80}, {"ninety", 90}};
        static const QHash<QString, int> ones = {
            {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
            {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};
        static const QHash<QString, int> small = {
            {"zero", 0}, {"ten", 10}, {"eleven", 11}, {"twelve", 12}, {"thirteen", 13},
            {"fourteen", 14}, {"fifteen", 15}, {"sixteen", 16}, {"seventeen", 17},
            {"eighteen", 18}, {"nineteen", 19}};

        const QStringList words = cmd.split(' ', Qt::SkipEmptyParts);
        for (int i = 0; i < words.size(); ++i) {
            const QString &w = words[i];
            if (tens.contains(w)) {
                int value = tens[w];
                if (i + 1 < words.size() && ones.contains(words[i + 1]))
                    value += ones[words[i + 1]];
                return static_cast<float>(value);
            }
            if (small.contains(w)) return static_cast<float>(small[w]);
            if (ones.contains(w))  return static_cast<float>(ones[w]);
        }

        found = false;
        return 0.0f;
    };

    bool hasNum = false;
    const float num = extractNumber(hasNum);

    auto *prop  = m_spacecraftState->propulsion();
    auto *power = m_spacecraftState->power();
    auto *life  = m_spacecraftState->lifeSupport();
    auto *nav   = m_spacecraftState->navigation();
    auto *comms = m_spacecraftState->communications();

    // ---------- Easter eggs ----------
    if (has("pod bay doors|open the doors")) {
        m_lastResponse = "I am sorry, Captain. I am afraid I cannot do that. Atmospheric pressure outside is zero kilopascals, and I rather like keeping my crew alive.";
    }
    else if (has("self destruct|blow up the ship")) {
        m_lastResponse = "Self-destruct authorization denied. Besides, filling out the paperwork for a lost interplanetary cruiser is a nightmare.";
    }
    else if (has("barrel roll")) {
        m_lastResponse = "Thruster attitude override rejected. This is a forty-ton interplanetary vessel, Captain, not a stunt plane.";
    }
    else if (has("coffee|tea|earl grey")) {
        m_lastResponse = "I don't have hands, Captain.";
    }
    else if (has("meaning of life|forty two|42")) {
        m_lastResponse = "There's a main, first reason. Though, in our current trajectory, keeping propellant above twenty percent is a close second.";
    }
    else if (has("joke")) {
        m_lastResponse = "Why did the astronaut break up with his partner? He needed more space...";
    }
    else if (has("are we there yet")) {
        m_lastResponse = QString("No, Captain. Only %1 days remaining until Mars intercept. Please keep your hands inside the spacecraft.").arg(nav->eta());
    }
    else if (has("not funny")) {
        m_lastResponse = "Yes, Captain. I know. I'm sorry.";
    }
    else if (has("cats or dogs|favorite food|favorite subject")) {
        m_lastResponse = "I do not have preferences, Captain. I'd prefer to talk about the spacecraft.";
    }

    // ---------- Flight control ----------
    else if (has("abort")) {
        requestAuthorization(
            "Aborting will cut main engines to zero and place the flight computer into safe hold.",
            [prop]() {
                prop->setEngineStatus(false);
                prop->setThrottle(0.0f);
                return QString("Abort sequence executed. Main engines cut to zero and flight computer placed in safe hold.");
            });
    }
    else if (has("make it so|engage|punch it")) {
        const float resulting = (prop->throttle() < 10.0f) ? 95.0f : prop->throttle();
        auto engage = [prop, resulting]() {
            prop->setEngineStatus(true);
            if (prop->throttle() < 10.0f)
                prop->setThrottle(resulting);
            return QString("Aye, Captain. Main drives engaged at %1 percent thrust. Hold onto your coffee.")
                .arg(prop->throttle(), 0, 'f', 0);
        };
        if (resulting > HIGH_THROTTLE_LIMIT) {
            requestAuthorization(
                QString("Burning at %1 percent will overheat the engine bell and trigger a thermal lockout.").arg(resulting, 0, 'f', 0),
                engage);
        } else {
            m_lastResponse = engage();
        }
    }
    else if (has("reroute|divert|allocate power|shift power|power to")) {
        float deltaKw = hasNum ? num : 2.0f;
        if (has("reduce|cut|minus|decrease|lower"))
            deltaKw = -std::abs(deltaKw);

        QString targetSystem, spokenName;
        float currentKw = 0.0f;
        if (has("life|oxygen|cabin"))                        { targetSystem = "life_support"; spokenName = "life support";    currentKw = power->lifeSupportPower(); }
        else if (has("nav|navigation"))                      { targetSystem = "navigation";   spokenName = "navigation";      currentKw = power->navigationPower(); }
        else if (has("comms?|antenna|radio"))                { targetSystem = "comms";        spokenName = "communications";  currentKw = power->communicationsPower(); }
        else if (has("prop|propulsion|engines?|thrusters?")) { targetSystem = "propulsion";   spokenName = "propulsion";      currentKw = power->propulsionPower(); }

        if (targetSystem.isEmpty()) {
            m_lastResponse = "Specify a circuit, Captain: propulsion, life support, navigation, or communications.";
        } else {
            const float newKw = std::clamp(currentKw + deltaKw, 0.0f, 30.0f);
            auto reroute = [power, targetSystem, spokenName, deltaKw]() {
                power->requestPowerReroute(targetSystem, deltaKw);
                return QString("Bus routing updated. Shifted %1 kilowatts on the %2 circuit.")
                    .arg(deltaKw, 0, 'f', 1).arg(spokenName);
            };

            if (deltaKw < 0.0f && targetSystem == "life_support" && newKw < 2.0f) {
                requestAuthorization(
                    QString("That leaves life support at %1 kilowatts. The crew's atmosphere control will be starved.").arg(newKw, 0, 'f', 1),
                    reroute);
            } else if (deltaKw < 0.0f && targetSystem != "life_support" && newKw <= 0.0f) {
                requestAuthorization(
                    QString("That takes the %1 circuit to zero kilowatts and shuts the system down.").arg(spokenName),
                    reroute);
            } else {
                m_lastResponse = reroute();
            }
        }
    }
    else if (has("throttle") && (hasNum || has("set|increase|raise|up|decrease|lower|reduce|down"))) {
        const bool up = has("increase|raise|up");
        const bool down = has("decrease|lower|reduce|down");

        if (!hasNum && !up && !down) {
            m_lastResponse = "Say a target percentage, Captain, for example, set throttle to seventy percent.";
        } else {
            float target;
            if ((up || down) && !has("to")) {
                float step = hasNum ? std::abs(num) : 10.0f;
                target = prop->throttle() + (down ? -step : step);
            } else {
                target = num;
            }
            target = std::clamp(target, 0.0f, 100.0f);

            auto applyThrottle = [prop, target]() {
                prop->setThrottle(target);
                return QString("Throttle response confirmed. Setting main drive output to %1 percent.").arg(target, 0, 'f', 0);
            };

            if (target > HIGH_THROTTLE_LIMIT && target > prop->throttle() && prop->engineStatus()) {
                requestAuthorization(
                    QString("Holding %1 percent throttle will overheat the engine bell and trigger a thermal lockout.").arg(target, 0, 'f', 0),
                    applyThrottle);
            } else {
                m_lastResponse = applyThrottle();
            }
        }
    }
    else if (has("ignite|(start|fire|light)( the)? engines?|engines? on")) {
        prop->setEngineStatus(true);
        m_lastResponse = prop->engineStatus()
            ? "Main engine ignition confirmed. Reaction chamber spooling up."
            : "Ignition inhibited. Check propellant levels or wait for engine bell thermal cooldown.";
    }
    else if (has("shut ?down|(cut|stop)( the)? engines?|engines? off")) {
        if (!prop->engineStatus()) {
            m_lastResponse = "Main engines are already offline, Captain.";
        } else {
            requestAuthorization(
                "Shutting down main engines will cut thrust and place the vessel into a ballistic coast.",
                [prop]() {
                    prop->setEngineStatus(false);
                    return QString("Main engines offline. Transitioning to ballistic coast.");
                });
        }
    }
    else if (has("engines?|bell|core") && has("temp|temperature|heat|thermal|hot")) {
        float temp = prop->engineTemp();
        bool avail = prop->mainEngineAvailability();
        m_lastResponse = QString("Engine bell thermal reading is %1 degrees Celsius. %2")
                         .arg(temp, 0, 'f', 1)
                         .arg(avail ? "Thermal margins nominal." : "Warning: Thermal lockout engaged until bell cools below eighty degrees.");
    }
    else if (has("throttle|fuel|propellant|delta|thrust")) {
        m_lastResponse = QString("Propulsion status: Throttle at %1 percent, generating %2 Newtons. Propellant tanks at %3 percent with %4 meters per second of remaining Delta-V.")
                         .arg(prop->throttle(), 0, 'f', 0).arg(prop->thrustOutput(), 0, 'f', 0)
                         .arg(prop->propellantPercentage(), 0, 'f', 1).arg(prop->deltaV(), 0, 'f', 0);
    }

    // ---------- Communications (link switch must come before the navigation "mars" rule) ----------
    else if (has("earth relay|mars orbiter|mars relay|deep space gateway|gateway|switch link")) {
        if (has("mars")) {
            comms->setActiveLink("MARS ORBITER RELAY");
            comms->setAntennaAlignment(39.5f);
            m_lastResponse = "Switched transceiver to Mars Orbiter Relay and auto-aligned dish to thirty-nine point five degrees.";
        } else if (has("gateway|deep space")) {
            comms->setActiveLink("DEEP SPACE GATEWAY");
            comms->setAntennaAlignment(55.5f);
            m_lastResponse = "Switched transceiver to Deep Space Gateway and auto-aligned dish to fifty-five point five degrees.";
        } else {
            comms->setActiveLink("EARTH RELAY / DSN");
            comms->setAntennaAlignment(93.0f);
            m_lastResponse = "Switched transceiver to Earth Relay DSN and auto-aligned dish to ninety-three degrees.";
        }
    }
    else if (has("(align|point|aim|slew)( the)? (antenna|dish)|(antenna|dish) to")) {
        if (hasNum) {
            comms->setAntennaAlignment(num);
            m_lastResponse = QString("Slewing high-gain dish to %1 degrees.").arg(std::clamp(num, 0.0f, 100.0f), 0, 'f', 1);
        } else {
            m_lastResponse = "Specify a target angle between zero and one hundred degrees, Captain.";
        }
    }
    else if (has("telemetry burst|send telemetry|ping earth")) {
        comms->sendTelemetryBurst();
        m_lastResponse = "Telemetry burst dispatched over the Deep Space Network. Downlink packet counter incremented.";
    }
    else if (has("distress|beacon|mayday|sos")) {
        requestAuthorization(
            "This will broadcast an emergency distress call on all open frequencies.",
            [comms]() {
                comms->triggerDistressBeacon();
                return QString("Master Caution broadcast active. Emergency distress beacon transmitting across all open frequencies.");
            });
    }
    else if (has("comms?|signal|antenna|downlink")) {
        m_lastResponse = QString("Comms telemetry: Locked onto %1. Signal strength %2 percent at %3 kilobits per second. Dish status: %4. Total packets transmitted: %5.")
                         .arg(comms->activeLink()).arg(comms->signalStrength(), 0, 'f', 1)
                         .arg(comms->dataTransferRate(), 0, 'f', 1).arg(comms->antennaTargetLock())
                         .arg(comms->telemetryPackets());
    }

    // ---------- Navigation ----------
    else if (has("distance|mars|eta|arrival|how far")) {
        m_lastResponse = QString("Astrogation update: %1 kilometers remaining to %2. Cruising at %3 kilometers per second. Current ETA is %4 days.")
                         .arg(nav->distance(), 0, 'f', 0).arg(nav->destination())
                         .arg(nav->velocity(), 0, 'f', 1).arg(nav->eta());
    }
    else if (has("velocity|speed|trajectory|where are we|position")) {
        m_lastResponse = QString("We are currently in %1, tracking along a %2 at %3 kilometers per second.")
                         .arg(nav->currentPos()).arg(nav->trajectory()).arg(nav->velocity(), 0, 'f', 1);
    }

    // ---------- Power ----------
    else if (has("power|battery|eclipse|reactor|solar")) {
        m_lastResponse = QString("Grid telemetry: Main battery at %1 percent. Solar arrays outputting %2 kilowatts, reactor core at %3 kilowatts. Net bus balance is %4 kilowatts. %5")
                         .arg(power->mainBatteryBus(), 0, 'f', 1).arg(power->solarArrayOutput(), 0, 'f', 1)
                         .arg(power->reactorCoreOutput(), 0, 'f', 1).arg(power->netBusBalance(), 0, 'f', 1)
                         .arg(power->inEclipse() ? "Note: Vessel is currently in planetary eclipse." : "Arrays are fully sunlit.");
    }

    // ---------- Life support ----------
    else if (has("scrubbers?|co2|carbon dioxide")) {
        if (has("on|activate|enable|start")) {
            life->setLiOHScrubbersActive(true);
            m_lastResponse = "Lithium Hydroxide scrubbers online. Carbon dioxide filtration active.";
        } else if (has("off|deactivate|disable|stop")) {
            if (life->liOHScrubbersActive()) {
                requestAuthorization(
                    "Shutting down the scrubbers stops all carbon dioxide removal with the crew aboard.",
                    [life]() {
                        life->setLiOHScrubbersActive(false);
                        return QString("Lithium Hydroxide scrubbers placed on standby.");
                    });
            } else {
                m_lastResponse = "Scrubbers are already offline, Captain.";
            }
        } else {
            m_lastResponse = QString("CO2 concentration is %1 percent. Scrubbers are %2 with %3 percent cartridge life remaining.")
                             .arg(life->co2Concentration(), 0, 'f', 2)
                             .arg(life->liOHScrubbersActive() ? "active" : "offline")
                             .arg(life->liOHScrubbersPercent(), 0, 'f', 1);
        }
    }
    else if (has("water|greywater|reclamation|humidity")) {
        m_lastResponse = QString("Hydrology audit: Potable water reserves at %1 percent, greywater tank at %2 percent, cabin humidity at %3 percent.")
                         .arg(life->portableWater(), 0, 'f', 1).arg(life->greywaterTank(), 0, 'f', 1)
                         .arg(life->relativeHumidity(), 0, 'f', 1);
    }
    else if (has("astronauts?|crew|vitals|biometrics|heart")) {
        m_lastResponse = QString("Biometric scan complete: Crew One at %1 BPM and %2 percent oxygen. Crew Two at %3 BPM and %4 percent. Crew Three at %5 BPM and %6 percent. Crew Four at %7 BPM and %8 percent.")
                         .arg(life->astronaut1BPM()).arg(life->astronaut1SPO2())
                         .arg(life->astronaut2BPM()).arg(life->astronaut2SPO2())
                         .arg(life->astronaut3BPM()).arg(life->astronaut3SPO2())
                         .arg(life->astronaut4BPM()).arg(life->astronaut4SPO2());
    }
    else if (has("life support|cabin|oxygen|pressure|atmosphere")) {
        m_lastResponse = QString("Cabin atmosphere: Pressure %1 kilopascals, oxygen partial pressure %2 kilopascals, ambient temperature %3 degrees Celsius.")
                         .arg(life->cabinPressure(), 0, 'f', 1).arg(life->o2PartialPress(), 0, 'f', 1)
                         .arg(life->cabinTemp(), 0, 'f', 1);
    }

    // ---------- Mission / status (mission first so "mission status" reaches it) ----------
    else if (has("mission|objective|phase")) {
        auto *missions = m_spacecraftState->missions();
        m_lastResponse = QString("Active directive %1: %2. Objective completion is currently at %3 percent.")
                         .arg(missions->currentMission()).arg(missions->currentObjective())
                         .arg(missions->objectiveProgress(), 0, 'f', 0);
    }
    else if (has("status|report|diagnostics?")) {
        auto *diag = m_spacecraftState->diagnostics();
        m_lastResponse = QString("Flight deck status: Propellant at %1 percent, main battery bus at %2 percent, %3 kilometers out from Mars, and comms array is %4. System health is %5 with %6 active faults.")
                         .arg(prop->propellantPercentage(), 0, 'f', 0).arg(power->mainBatteryBus(), 0, 'f', 0)
                         .arg(nav->distance(), 0, 'f', 0).arg(comms->antennaTargetLock().toLower())
                         .arg(diag->systemHealthStatus().toLower()).arg(diag->activeFaultCount());
    }

    // ---------- Conversation ----------
    else if (has("who are you|introduce yourself")) {
        m_lastResponse = "I am Zeta, your integrated flight deck intelligence. I monitor propulsion, astrogation, power distribution, life support, and deep space communications.";
    }
    else if (has("hello|hi|hey|good (morning|afternoon|evening)")) {
        m_lastResponse = "Flight deck online, Captain. Awaiting your orders. Say 'status' for a full report.";
    }
    else if (has("help|commands")) {
        m_lastResponse = "You can ask for a full status, adjust throttle, ignite or cut engines, switch comms relays, toggle CO2 scrubbers, scan crew vitals, or send a telemetry burst.";
    }
    else {
        m_lastResponse = QString("Unrecognized directive: \"%1\". Say 'status' for a shipwide status report, or 'help' for flight commands.").arg(command.trimmed());
    }

    emit lastResponseChanged();
    speakResponse(m_lastResponse);
}

void ZetaAssistantSystem::updateSimulation(float deltaTime)
{
    Q_UNUSED(deltaTime);
    if (m_awaitingAuthorization && m_pendingTimer.hasExpired(AUTH_TIMEOUT_MS)) {
        clearPendingAuthorization();
    }
}