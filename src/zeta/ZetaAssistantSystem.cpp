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
#include <QDebug>

ZetaAssistantSystem::ZetaAssistantSystem(QObject *parent)
    : QObject(parent)
{
    m_inputWavPath = QDir::temp().filePath("zeta_stt_input.wav");
    m_outputWavPath = QDir::temp().filePath("zeta_tts_output.wav");

    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_audioOutput->setVolume(1.0f);
    m_mediaPlayer->setAudioOutput(m_audioOutput);

    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state)
            {
        bool speaking = (state == QMediaPlayer::PlayingState);
        if (m_isSpeaking != speaking) {
            m_isSpeaking = speaking;
            emit isSpeakingChanged();
        }
        if (!speaking && !m_isListening) {
            m_audioStatus = "STANDBY";
            emit audioStatusChanged();
        } });

    m_piperProcess = new QProcess(this);
    connect(m_piperProcess, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus)
            {
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
        } });

    m_whisperProcess = new QProcess(this);
    connect(m_whisperProcess, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus)
            {
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
        } });
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

void ZetaAssistantSystem::startListening() {
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

void ZetaAssistantSystem::processCommand(const QString &command)
{
    QString cmd = command.trimmed().toLower();
    cmd.remove(QRegularExpression("[.,!?]"));

    if (!m_spacecraftState)
    {
        m_lastResponse = "Critical fault: Flight control backplane is offline. I am flying blind here, Captain.";
        emit lastResponseChanged();
        speakResponse(m_lastResponse);
        return;
    }

    auto extractPercentage = [&cmd](bool &found) -> float {
        found = false;
        QRegularExpression numRe("(-?\\d+(?:\\.\\d+)?)");
        QRegularExpressionMatch match = numRe.match(cmd);
        if (match.hasMatch()) {
            found = true;
            return match.captured(1).toFloat();
        }
        if (cmd.contains("full") || cmd.contains("max") || cmd.contains("hundred")) { found = true; return 100.0f; }
        if (cmd.contains("ninety"))  { found = true; return 90.0f; }
        if (cmd.contains("eighty"))  { found = true; return 80.0f; }
        if (cmd.contains("seventy")) { found = true; return 70.0f; }
        if (cmd.contains("sixty"))   { found = true; return 60.0f; }
        if (cmd.contains("fifty") || cmd.contains("half")) { found = true; return 50.0f; }
        if (cmd.contains("forty"))   { found = true; return 40.0f; }
        if (cmd.contains("thirty"))  { found = true; return 30.0f; }
        if (cmd.contains("twenty"))  { found = true; return 20.0f; }
        if (cmd.contains("ten"))     { found = true; return 10.0f; }
        if (cmd.contains("zero") || cmd.contains("idle")) { found = true; return 0.0f; }
        return 0.0f;
    };

    if (cmd.contains("pod bay doors") || cmd.contains("open the doors")) {
        m_lastResponse = "I am sorry, Captain. I am afraid I cannot do that. Atmospheric pressure outside is zero kilopascals, and I rather like keeping my crew alive.";
    }
    else if (cmd.contains("self-destruct") || cmd.contains("blow up the ship")) {
        m_lastResponse = "Self-destruct authorization denied. Besides, filling out the paperwork for a lost interplanetary cruiser is a nightmare.";
    }
    else if (cmd.contains("make it so") || cmd.contains("engage") || cmd.contains("punch it")) {
        m_spacecraftState->propulsion()->setEngineStatus(true);
        if (m_spacecraftState->propulsion()->throttle() < 10.0f) {
            m_spacecraftState->propulsion()->setThrottle(95.0f);
        }
        m_lastResponse = "Aye, Captain. Main drives engaged at ninety-five percent thrust. Hold onto your coffee.";
    }
    else if (cmd.contains("do a barrel roll") || cmd.contains("barrel roll")) {
        m_lastResponse = "Thruster attitude override rejected. This is a forty-ton interplanetary vessel, Captain, not a stunt plane.";
    }
    else if (cmd.contains("coffee") || cmd.contains("tea") || cmd.contains("earl grey")) {
        m_lastResponse = "I don't have hands, Captain.";
    }
    else if (cmd.contains("meaning of life") || cmd.contains("forty two") || cmd.contains("42")) {
        m_lastResponse = "There's a main, first reason. Though, in our current trajectory, keeping propellant above twenty percent is a close second.";
    }
    else if (cmd.contains("tell me a joke") || cmd.contains("joke")) {
        m_lastResponse = "Why did the astronaut break up with his partner? He needed more space...";
    }
    else if (cmd.contains("are we there yet")) {
        int etaDays = m_spacecraftState->navigation()->eta();
        m_lastResponse = QString("No, Captain. Only %1 days remaining until Mars intercept. Please keep your hands inside the spacecraft.").arg(etaDays);
    }
    else if (cmd.contains("not funny")) {
        m_lastResponse = QString("Yes, Captain. I know. I'm sorry.");
    }
    else if (cmd.contains("cats or dogs") || cmd.contains("favorite food") || cmd.contains("favorite subject")) {
        m_lastResponse = QString("I do not have preferences, Captain. I'd prefer to talk about the spacecraft.");
    }

    else if (cmd.contains("throttle") && (cmd.contains("set") || cmd.contains("to") || cmd.contains("increase") || cmd.contains("decrease") || cmd.contains("full") || cmd.contains("max") || cmd.contains("zero"))) {
        bool hasVal = false;
        float val = extractPercentage(hasVal);
        if (hasVal) {
            val = std::clamp(val, 0.0f, 100.0f);
            m_spacecraftState->propulsion()->setThrottle(val);
            m_lastResponse = QString("Throttle response confirmed. Setting main drive output to %1 percent.").arg(val, 0, 'f', 0);
        } else {
            m_lastResponse = "Say a target percentage, Captain—for example, set throttle to seventy percent.";
        }
    }
    else if (cmd.contains("ignite") || cmd.contains("start engine") || cmd.contains("engines on") || cmd.contains("fire engine")) {
        m_spacecraftState->propulsion()->setEngineStatus(true);
        bool active = m_spacecraftState->propulsion()->engineStatus();
        m_lastResponse = active
            ? "Main engine ignition confirmed. Reaction chamber spooling up."
            : "Ignition inhibited. Check propellant levels or wait for engine bell thermal cooldown.";
    }
    else if (cmd.contains("shutdown") || cmd.contains("cut engine") || cmd.contains("stop engine") || cmd.contains("engines off")) {
        m_spacecraftState->propulsion()->setEngineStatus(false);
        m_lastResponse = "Main engines offline. Transitioning to ballistic coast.";
    }
    else if ((cmd.contains("engine") || cmd.contains("bell") || cmd.contains("core")) && (cmd.contains("temp") || cmd.contains("heat") || cmd.contains("thermal") || cmd.contains("hot"))) {
        float temp = m_spacecraftState->propulsion()->engineTemp();
        bool avail = m_spacecraftState->propulsion()->mainEngineAvailability();
        m_lastResponse = QString("Engine bell thermal reading is %1 degrees Celsius. %2")
                         .arg(temp, 0, 'f', 1)
                         .arg(avail ? "Thermal margins nominal." : "Warning: Thermal lockout engaged until bell cools below eighty degrees.");
    }
    else if (cmd.contains("throttle") || cmd.contains("fuel") || cmd.contains("propellant") || cmd.contains("delta") || cmd.contains("thrust")) {
        float fuel = m_spacecraftState->propulsion()->propellantPercentage();
        float thrust = m_spacecraftState->propulsion()->thrustOutput();
        float deltav = m_spacecraftState->propulsion()->deltaV();
        float throttle = m_spacecraftState->propulsion()->throttle();
        m_lastResponse = QString("Propulsion status: Throttle at %1 percent, generating %2 Newtons. Propellant tanks at %3 percent with %4 meters per second of remaining Delta-V.")
                         .arg(throttle, 0, 'f', 0).arg(thrust, 0, 'f', 0).arg(fuel, 0, 'f', 1).arg(deltav, 0, 'f', 0);
    }

    else if (cmd.contains("distance") || cmd.contains("mars") || cmd.contains("eta") || cmd.contains("arrival")) {
        float dist = m_spacecraftState->navigation()->distance();
        float vel = m_spacecraftState->navigation()->velocity();
        int etaDays = m_spacecraftState->navigation()->eta();
        QString dest = m_spacecraftState->navigation()->destination();
        m_lastResponse = QString("Astrogation update: %1 kilometers remaining to %2. Cruising at %3 kilometers per second. Current ETA is %4 days.")
                         .arg(dist, 0, 'f', 0).arg(dest).arg(vel, 0, 'f', 1).arg(etaDays);
    }
    else if (cmd.contains("velocity") || cmd.contains("speed") || cmd.contains("trajectory") || cmd.contains("where are we") || cmd.contains("position")) {
        QString traj = m_spacecraftState->navigation()->trajectory();
        QString pos = m_spacecraftState->navigation()->currentPos();
        float vel = m_spacecraftState->navigation()->velocity();
        m_lastResponse = QString("We are currently in %1, tracking along a %2 at %3 kilometers per second.")
                         .arg(pos).arg(traj).arg(vel, 0, 'f', 1);
    }

    else if (cmd.contains("reroute") || cmd.contains("allocate power") || cmd.contains("power to")) {
        bool hasVal = false;
        float deltaKw = extractPercentage(hasVal);
        if (!hasVal) deltaKw = 2.0f;
        if (cmd.contains("reduce") || cmd.contains("cut") || cmd.contains("minus")) deltaKw = -std::abs(deltaKw);

        QString targetSystem = "propulsion";
        QString spokenName = "propulsion";
        if (cmd.contains("life") || cmd.contains("oxygen") || cmd.contains("cabin")) { targetSystem = "life_support"; spokenName = "life support"; }
        else if (cmd.contains("nav")) { targetSystem = "navigation"; spokenName = "navigation"; }
        else if (cmd.contains("comm") || cmd.contains("antenna") || cmd.contains("radio")) { targetSystem = "comms"; spokenName = "communications"; }

        m_spacecraftState->power()->requestPowerReroute(targetSystem, deltaKw);
        m_lastResponse = QString("Bus routing updated. Shifted %1 kilowatts on the %2 circuit.").arg(deltaKw, 0, 'f', 1).arg(spokenName);
    }
    else if (cmd.contains("power") || cmd.contains("battery") || cmd.contains("eclipse") || cmd.contains("reactor") || cmd.contains("solar")) {
        float mainBus = m_spacecraftState->power()->mainBatteryBus();
        float solar = m_spacecraftState->power()->solarArrayOutput();
        float reactor = m_spacecraftState->power()->reactorCoreOutput();
        float net = m_spacecraftState->power()->netBusBalance();
        bool eclipse = m_spacecraftState->power()->inEclipse();
        m_lastResponse = QString("Grid telemetry: Main battery at %1 percent. Solar arrays outputting %2 kilowatts, reactor core at %3 kilowatts. Net bus balance is %4 kilowatts. %5")
                         .arg(mainBus, 0, 'f', 1).arg(solar, 0, 'f', 1).arg(reactor, 0, 'f', 1).arg(net, 0, 'f', 1)
                         .arg(eclipse ? "Note: Vessel is currently in planetary eclipse." : "Arrays are fully sunlit.");
    }

    else if (cmd.contains("scrubber") || cmd.contains("co2") || cmd.contains("carbon dioxide")) {
        if (cmd.contains("on") || cmd.contains("activate") || cmd.contains("enable") || cmd.contains("start")) {
            m_spacecraftState->lifeSupport()->setLiOHScrubbersActive(true);
            m_lastResponse = "Lithium Hydroxide scrubbers online. Carbon dioxide filtration active.";
        } else if (cmd.contains("off") || cmd.contains("deactivate") || cmd.contains("disable") || cmd.contains("stop")) {
            m_spacecraftState->lifeSupport()->setLiOHScrubbersActive(false);
            m_lastResponse = "Lithium Hydroxide scrubbers placed on standby.";
        } else {
            bool active = m_spacecraftState->lifeSupport()->liOHScrubbersActive();
            float pct = m_spacecraftState->lifeSupport()->liOHScrubbersPercent();
            float co2 = m_spacecraftState->lifeSupport()->co2Concentration();
            m_lastResponse = QString("CO2 concentration is %1 percent. Scrubbers are %2 with %3 percent cartridge life remaining.")
                             .arg(co2, 0, 'f', 2).arg(active ? "active" : "offline").arg(pct, 0, 'f', 1);
        }
    }
    else if (cmd.contains("water") || cmd.contains("greywater") || cmd.contains("reclamation") || cmd.contains("humidity")) {
        float portable = m_spacecraftState->lifeSupport()->portableWater();
        float grey = m_spacecraftState->lifeSupport()->greywaterTank();
        float hum = m_spacecraftState->lifeSupport()->relativeHumidity();
        m_lastResponse = QString("Hydrology audit: Potable water reserves at %1 percent, greywater tank at %2 percent, cabin humidity at %3 percent.")
                         .arg(portable, 0, 'f', 1).arg(grey, 0, 'f', 1).arg(hum, 0, 'f', 1);
    }
    else if (cmd.contains("astronaut") || cmd.contains("crew") || cmd.contains("vitals") || cmd.contains("biometrics") || cmd.contains("heart")) {
        int bpm1 = m_spacecraftState->lifeSupport()->astronaut1BPM();
        int spo21 = m_spacecraftState->lifeSupport()->astronaut1SPO2();
        int bpm2 = m_spacecraftState->lifeSupport()->astronaut2BPM();
        int spo22 = m_spacecraftState->lifeSupport()->astronaut2SPO2();
        int bpm3 = m_spacecraftState->lifeSupport()->astronaut3BPM();
        int bpm4 = m_spacecraftState->lifeSupport()->astronaut4BPM();
        m_lastResponse = QString("Biometric scan complete: Crew One at %1 BPM and %2 percent oxygen. Crew Two at %3 BPM and %4 percent. Crew Three and Four holding steady at %5 and %6 BPM.")
                         .arg(bpm1).arg(spo21).arg(bpm2).arg(spo22).arg(bpm3).arg(bpm4);
    }
    else if (cmd.contains("life support") || cmd.contains("cabin") || cmd.contains("oxygen") || cmd.contains("pressure") || cmd.contains("atmosphere")) {
        float pressure = m_spacecraftState->lifeSupport()->cabinPressure();
        float o2 = m_spacecraftState->lifeSupport()->o2PartialPress();
        float temp = m_spacecraftState->lifeSupport()->cabinTemp();
        m_lastResponse = QString("Cabin atmosphere nominal: Pressure %1 kilopascals, oxygen partial pressure %2 kilopascals, ambient temperature %3 degrees Celsius.")
                         .arg(pressure, 0, 'f', 1).arg(o2, 0, 'f', 1).arg(temp, 0, 'f', 1);
    }

    else if (cmd.contains("align antenna") || cmd.contains("antenna to") || cmd.contains("point antenna")) {
        bool hasVal = false;
        float deg = extractPercentage(hasVal);
        if (hasVal) {
            m_spacecraftState->communications()->setAntennaAlignment(deg);
            m_lastResponse = QString("Slewing high-gain dish to %1 degrees.").arg(deg, 0, 'f', 1);
        } else {
            m_lastResponse = "Specify a target angle between zero and one hundred degrees, Captain.";
        }
    }
    else if (cmd.contains("earth relay") || cmd.contains("mars orbiter") || cmd.contains("deep space gateway") || cmd.contains("switch link")) {
        if (cmd.contains("mars")) {
            m_spacecraftState->communications()->setActiveLink("MARS ORBITER RELAY");
            m_spacecraftState->communications()->setAntennaAlignment(39.5f);
            m_lastResponse = "Switched transceiver to Mars Orbiter Relay and auto-aligned dish to thirty-nine point five degrees.";
        } else if (cmd.contains("gateway") || cmd.contains("deep space")) {
            m_spacecraftState->communications()->setActiveLink("DEEP SPACE GATEWAY");
            m_spacecraftState->communications()->setAntennaAlignment(55.5f);
            m_lastResponse = "Switched transceiver to Deep Space Gateway and auto-aligned dish to fifty-five point five degrees.";
        } else {
            m_spacecraftState->communications()->setActiveLink("EARTH RELAY / DSN");
            m_spacecraftState->communications()->setAntennaAlignment(93.0f);
            m_lastResponse = "Switched transceiver to Earth Relay DSN and auto-aligned dish to ninety-three degrees.";
        }
    }
    else if (cmd.contains("telemetry burst") || cmd.contains("send telemetry") || cmd.contains("ping earth")) {
        m_spacecraftState->communications()->sendTelemetryBurst();
        m_lastResponse = "Telemetry burst dispatched over the Deep Space Network. Downlink packet counter incremented.";
    }
    else if (cmd.contains("distress") || cmd.contains("beacon") || cmd.contains("mayday") || cmd.contains("sos")) {
        m_spacecraftState->communications()->triggerDistressBeacon();
        m_lastResponse = "Master Caution broadcast active. Emergency distress beacon transmitting across all open frequencies.";
    }
    else if (cmd.contains("comms") || cmd.contains("signal") || cmd.contains("antenna") || cmd.contains("downlink")) {
        float signal = m_spacecraftState->communications()->signalStrength();
        float rate = m_spacecraftState->communications()->dataTransferRate();
        QString lock = m_spacecraftState->communications()->antennaTargetLock();
        QString link = m_spacecraftState->communications()->activeLink();
        int packets = m_spacecraftState->communications()->telemetryPackets();
        m_lastResponse = QString("Comms telemetry: Locked onto %1. Signal strength %2 percent at %3 kilobits per second. Dish status: %4. Total packets transmitted: %5.")
                         .arg(link).arg(signal, 0, 'f', 1).arg(rate, 0, 'f', 1).arg(lock).arg(packets);
    }

    else if (cmd.contains("status") || cmd.contains("report") || cmd.contains("diagnostic")) {
        float fuel = m_spacecraftState->propulsion()->propellantPercentage();
        float batt = m_spacecraftState->power()->mainBatteryBus();
        float dist = m_spacecraftState->navigation()->distance();
        QString lock = m_spacecraftState->communications()->antennaTargetLock();
        m_lastResponse = QString("Flight deck status: Propellant at %1 percent, main battery bus at %2 percent, %3 kilometers out from Mars, and comms array is %4. All primary systems green.")
                         .arg(fuel, 0, 'f', 0).arg(batt, 0, 'f', 0).arg(dist, 0, 'f', 0).arg(lock.toLower());
    }
    else if (cmd.contains("abort")) {
        m_spacecraftState->propulsion()->setEngineStatus(false);
        m_spacecraftState->propulsion()->setThrottle(0.0f);
        m_lastResponse = "Abort sequence executed. Main engines cut to zero and flight computer placed in safe hold.";
    }
    else if (cmd.contains("mission") || cmd.contains("objective") || cmd.contains("phase")) {
        QString currentMission = m_spacecraftState->missions()->currentMission();
        QString objective = m_spacecraftState->missions()->currentObjective();
        float progress = m_spacecraftState->missions()->objectiveProgress();
        m_lastResponse = QString("Active directive %1: %2. Objective completion is currently at %3 percent.")
                         .arg(currentMission).arg(objective).arg(progress, 0, 'f', 0);
    }
    else if (cmd.contains("who are you") || cmd.contains("introduce yourself")) {
        m_lastResponse = "I am Zeta, your integrated flight deck intelligence. I monitor propulsion, astrogation, power distribution, life support, and deep space communications.";
    }
    else if (cmd.contains("hello") || cmd.contains("hi zeta") || cmd.contains("hey zeta") || cmd.contains("good morning")) {
        m_lastResponse = "Flight deck online, Captain. Telemetry buses are green and awaiting your orders.";
    }
    else if (cmd.contains("help") || cmd.contains("commands")) {
        m_lastResponse = "You can ask for a full status, adjust throttle, ignite or cut engines, switch comms relays, toggle CO2 scrubbers, scan crew vitals, or send a telemetry burst.";
    }
    else {
        m_lastResponse = QString("Unrecognized directive: \"%1\". Say 'status' for a shipwide status report, or 'help' for flight commands.").arg(command.trimmed());
    }

    emit lastResponseChanged();
    speakResponse(m_lastResponse);
}

void ZetaAssistantSystem::updateSimulation(float deltaTime) {}