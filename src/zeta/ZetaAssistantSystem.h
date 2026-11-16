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
#include <qqmlintegration.h>

class SpacecraftState;

class ZetaAssistantSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString lastResponse READ lastResponse NOTIFY lastResponseChanged)
    Q_PROPERTY(bool isListening READ isListening NOTIFY isListeningChanged)
    Q_PROPERTY(bool isSpeaking READ isSpeaking NOTIFY isSpeakingChanged)
    Q_PROPERTY(QString audioStatus READ audioStatus NOTIFY audioStatusChanged)

public:
    explicit ZetaAssistantSystem(QObject *parent = nullptr);
    ~ZetaAssistantSystem();

    void setSpacecraftState(SpacecraftState *state);

    QString lastResponse() const;
    bool isListening() const;
    bool isSpeaking() const;
    QString audioStatus() const;

    Q_INVOKABLE void startListening();
    Q_INVOKABLE void stopListening();
    Q_INVOKABLE void processCommand(const QString &command);
    Q_INVOKABLE void speakResponse(const QString &text);
    Q_INVOKABLE void updateSimulation(float deltaTime);

signals:
    void lastResponseChanged();
    void isListeningChanged();
    void isSpeakingChanged();
    void audioStatusChanged();

private:
    void processAudioBuffer(const QByteArray &data);
    QString resolveModelPath() const;

    static constexpr int ZETA_SPEAKER_ID = 10;

    QString m_lastResponse = "Zeta Flight Voice Link active. Neural synthesis online, Captain.";
    QString m_audioStatus = "STANDBY";
    bool m_isListening = false;
    bool m_isSpeaking = false;

    QAudioSource *m_audioSource = nullptr;
    QBuffer m_audioBuffer;
    QByteArray m_recordedAudioData;
    SpacecraftState *m_spacecraftState = nullptr;

    QProcess *m_piperProcess = nullptr;
    QMediaPlayer *m_mediaPlayer = nullptr;
    QAudioOutput *m_audioOutput = nullptr;
    QString m_outputWavPath;
};

#endif // ZETAASSISTANTSYSTEM_H