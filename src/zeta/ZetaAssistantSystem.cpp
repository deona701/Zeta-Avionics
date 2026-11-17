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

    if (!m_spacecraftState)
    {
        m_lastResponse = "Error: Flight control bus not linked to Zeta.";
        emit lastResponseChanged();
        speakResponse(m_lastResponse);
        return;
    }

    if (cmd.contains("throttle") || cmd.contains("fuel") || cmd.contains("propellant"))
    {
        float fuel = m_spacecraftState->propulsion()->propellantPercentage();
        float thrust = m_spacecraftState->propulsion()->thrustOutput();
        m_lastResponse = QString("Propulsion audit: Fuel reserves at %1 percent. Current thrust output is %2 Newtons.")
                             .arg(fuel, 0, 'f', 1)
                             .arg(thrust, 0, 'f', 0);
    }
    else if (cmd.contains("engine") || cmd.contains("temp"))
    {
        float temp = m_spacecraftState->propulsion()->engineTemp();
        bool status = m_spacecraftState->propulsion()->engineStatus();
        m_lastResponse = QString("Engine status: Core temperature at %1 degrees Celsius. Status is %2.")
                             .arg(temp, 0, 'f', 1)
                             .arg(status ? "online and firing" : "standby idle");
    }
    else if (cmd.contains("distance") || cmd.contains("mars") || cmd.contains("eta"))
    {
        float dist = m_spacecraftState->navigation()->distance();
        float vel = m_spacecraftState->navigation()->velocity();
        int etaDays = m_spacecraftState->navigation()->eta();
        m_lastResponse = QString("Navigation intercept: Distance to Mars is %1 kilometers. Velocity is %2 kilometers per second. Estimated arrival in %3 days.")
                             .arg(dist, 0, 'f', 0)
                             .arg(vel, 0, 'f', 1)
                             .arg(etaDays);
    }
    else if (cmd.contains("velocity") || cmd.contains("speed") || cmd.contains("trajectory"))
    {
        QString traj = m_spacecraftState->navigation()->trajectory();
        float vel = m_spacecraftState->navigation()->velocity();
        m_lastResponse = QString("Trajectory vector is %1 at %2 kilometers per second.").arg(traj).arg(vel, 0, 'f', 1);
    }
    else if (cmd.contains("power") || cmd.contains("battery") || cmd.contains("eclipse"))
    {
        float mainBus = m_spacecraftState->power()->mainBatteryBus();
        bool eclipse = m_spacecraftState->power()->inEclipse();
        m_lastResponse = QString("Electrical grid: Main battery bus at %1 percent. Solar eclipse shadow is %2.")
                             .arg(mainBus, 0, 'f', 1)
                             .arg(eclipse ? "active in planetary shadow" : "inactive and sunlit");
    }
    else if (cmd.contains("life support") || cmd.contains("cabin") || cmd.contains("oxygen") || cmd.contains("vitals"))
    {
        float pressure = m_spacecraftState->lifeSupport()->cabinPressure();
        float o2 = m_spacecraftState->lifeSupport()->o2PartialPress();
        float temp = m_spacecraftState->lifeSupport()->cabinTemp();
        m_lastResponse = QString("Life support telemetry: Cabin pressure %1 psi, oxygen partial pressure %2 kilopascals, temperature %3 degrees Celsius.")
                             .arg(pressure, 0, 'f', 1)
                             .arg(o2, 0, 'f', 1)
                             .arg(temp, 0, 'f', 1);
    }
    else if (cmd.contains("comms") || cmd.contains("signal") || cmd.contains("antenna"))
    {
        float signal = m_spacecraftState->communications()->signalStrength();
        int packets = m_spacecraftState->communications()->telemetryPackets();
        m_lastResponse = QString("Deep Space Network comms: Signal strength at %1 decibels. Packets relayed: %2.").arg(signal, 0, 'f', 1).arg(packets);
    }
    else if (cmd.contains("status") || cmd.contains("report") || cmd.contains("diagnostic"))
    {
        m_lastResponse = "Full systems audit: Propulsion, navigation, power, and life support buses are operating within normal parameters.";
    }
    else if (cmd.contains("abort") || cmd.contains("emergency"))
    {
        m_lastResponse = "Critical warning: Emergency abort sequence staged across flight computer. Awaiting manual override.";
    }
    else if (cmd.contains("mission") || cmd.contains("objective") || cmd.contains("phase"))
    {
        QString currentMission = m_spacecraftState->missions()->currentMission();
        QString objective = m_spacecraftState->missions()->currentObjective();
        float progress = m_spacecraftState->missions()->objectiveProgress();
        m_lastResponse = QString("Flight directive %1: %2. Progress is at %3 percent.")
                             .arg(currentMission)
                             .arg(objective)
                             .arg(progress, 0, 'f', 0);
    }
    else if (cmd.contains("who are you") || cmd.contains("zeta"))
    {
        m_lastResponse = "I am Zeta, your dedicated spacecraft avionics and flight deck intelligence assistant.";
    }
    else if (cmd.contains("help") || cmd.contains("commands"))
    {
        m_lastResponse = "Available queries include status report, fuel reserves, mars distance, power grid, cabin vitals, mission objective, or emergency abort.";
    }
    else
    {
        m_lastResponse = "Command processed: " + command + ". Subsystem telemetry registers nominal response.";
    }

    emit lastResponseChanged();
    speakResponse(m_lastResponse);
}

void ZetaAssistantSystem::updateSimulation(float deltaTime) {}