#ifndef ZETAASSISTANTSYSTEM_H
#define ZETAASSISTANTSYSTEM_H

#include <QObject>
#include <QString>
#include <QAudioSource>
#include <QBuffer>
#include <QByteArray>
#include <QProcess>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QElapsedTimer>
#include <functional>
#include <qqmlintegration.h>

class SpacecraftState;

class ZetaAssistantSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString lastResponse READ lastResponse NOTIFY lastResponseChanged)
    Q_PROPERTY(QString lastHeardText READ lastHeardText NOTIFY lastHeardTextChanged)
    Q_PROPERTY(bool isListening READ isListening NOTIFY isListeningChanged)
    Q_PROPERTY(bool isSpeaking READ isSpeaking NOTIFY isSpeakingChanged)
    Q_PROPERTY(bool awaitingAuthorization READ awaitingAuthorization NOTIFY awaitingAuthorizationChanged)
    Q_PROPERTY(QString audioStatus READ audioStatus NOTIFY audioStatusChanged)

public:
    explicit ZetaAssistantSystem(QObject *parent = nullptr);
    ~ZetaAssistantSystem();

    void setSpacecraftState(SpacecraftState *state);

    QString lastResponse() const;
    QString lastHeardText() const;
    bool isListening() const;
    bool isSpeaking() const;
    bool awaitingAuthorization() const;
    QString audioStatus() const;

    Q_INVOKABLE void startListening();
    Q_INVOKABLE void stopListening();
    Q_INVOKABLE void processCommand(const QString &command);
    Q_INVOKABLE void speakResponse(const QString &text);
    Q_INVOKABLE void updateSimulation(float deltaTime);

    static constexpr int   AUTH_TIMEOUT_MS = 15000;
    static constexpr float HIGH_THROTTLE_LIMIT = 85.0f;

signals:
    void lastResponseChanged();
    void lastHeardTextChanged();
    void isListeningChanged();
    void isSpeakingChanged();
    void awaitingAuthorizationChanged();
    void audioStatusChanged();

private:
    void processAudioBuffer(const QByteArray &pcmData);
    bool writeWavFile(const QString &filePath, const QByteArray &pcmData, int sampleRate = 16000) const;
    void requestAuthorization(const QString &prompt, std::function<QString()> action);
    void clearPendingAuthorization();
    QString resolveAssetPath(const QString &relativePath) const;

    static constexpr int ZETA_SPEAKER_ID = 10;

    QString m_lastResponse = "Zeta Flight Voice Link active. Neural synthesis and speech recognition online, Captain.";
    QString m_lastHeardText = "";
    QString m_audioStatus = "STANDBY";
    bool m_isListening = false;
    bool m_isSpeaking = false;
    bool m_awaitingAuthorization = false;

    std::function<QString()> m_pendingAction;
    QElapsedTimer m_pendingTimer;

    QAudioSource *m_audioSource = nullptr;
    QBuffer m_audioBuffer;
    QByteArray m_recordedAudioData;
    SpacecraftState *m_spacecraftState = nullptr;

    QProcess *m_whisperProcess = nullptr;
    QProcess *m_piperProcess = nullptr;
    QMediaPlayer *m_mediaPlayer = nullptr;
    QAudioOutput *m_audioOutput = nullptr;

    QString m_inputWavPath;
    QString m_outputWavPath;
};

#endif // ZETAASSISTANTSYSTEM_H