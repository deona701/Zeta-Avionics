#ifndef COMMUNICATIONSSYSTEM_H
#define COMMUNICATIONSSYSTEM_H

#include <QObject>
#include <QString>
#include <qqmlintegration.h>

class QNetworkAccessManager;

class CommunicationsSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float signalStrength READ signalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(float dataTransferRate READ dataTransferRate NOTIFY dataTransferRateChanged)
    Q_PROPERTY(float antennaAlignment READ antennaAlignment WRITE setAntennaAlignment NOTIFY antennaAlignmentChanged)
    Q_PROPERTY(int telemetryPackets READ telemetryPackets NOTIFY telemetryPacketsChanged)
    Q_PROPERTY(bool downlinkStatus READ downlinkStatus WRITE setDownlinkStatus NOTIFY downlinkStatusChanged)
    Q_PROPERTY(QString antennaTargetLock READ antennaTargetLock NOTIFY antennaTargetLockChanged)
    Q_PROPERTY(QString activeLink READ activeLink WRITE setActiveLink NOTIFY activeLinkChanged)
    
public:
    explicit CommunicationsSystem(QObject *parent = nullptr);

    float signalStrength() const;
    float dataTransferRate() const;
    float antennaAlignment() const;
    int telemetryPackets() const;
    bool downlinkStatus() const;
    QString antennaTargetLock() const;
    QString activeLink() const;

    Q_INVOKABLE void updateSimulation(float deltaTime);
    Q_INVOKABLE void sendTelemetryBurst();
    Q_INVOKABLE void triggerDistressBeacon();

public slots:
    void setAntennaAlignment(float newAntennaAlignment);
    void setDownlinkStatus(bool newDownlinkStatus);
    void setActiveLink(QString newActiveLink);

signals:
    void signalStrengthChanged();
    void dataTransferRateChanged();
    void antennaAlignmentChanged();
    void telemetryPacketsChanged();
    void downlinkStatusChanged();
    void antennaTargetLockChanged();
    void activeLinkChanged();

private:
    QNetworkAccessManager *m_networkManager;
    void sendNtfyNotification(const QString &title, const QString &message);

    float m_signalStrength = 90.0f;
    float m_dataTransferRate = 00.0f;
    float m_antennaAlignment = 91.7f;
    int m_telemetryPackets = 0;
    bool m_downlinkStatus = true;
    QString m_antennaTargetLock = "LOCKED";
    QString m_activeLink = "EARTH RELAY / DSN";
};

#endif // COMMUNICATIONSSYSTEM_H