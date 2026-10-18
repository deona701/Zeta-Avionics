#include "CommunicationsSystem.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

CommunicationsSystem::CommunicationsSystem(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_signalStrength(90.0f)
    , m_dataTransferRate(00.0f)
    , m_telemetryPackets(0)
    , m_downlinkStatus(true)
    , m_antennaTargetLock("LOCKED")
    , m_activeLink("EARTH RELAY")
{
}

float CommunicationsSystem::signalStrength() const { return m_signalStrength; }
float CommunicationsSystem::dataTransferRate() const { return m_dataTransferRate; }
int CommunicationsSystem::telemetryPackets() const { return m_telemetryPackets; }
bool CommunicationsSystem::downlinkStatus() const { return m_downlinkStatus; }
QString CommunicationsSystem::antennaTargetLock() const { return m_antennaTargetLock; }
QString CommunicationsSystem::activeLink() const { return m_activeLink; }


void CommunicationsSystem::sendNtfyNotification(const QString &title, const QString &message) {
    QUrl url("https://ntfy.sh/zeta_avionics");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    request.setRawHeader("Title", title.toUtf8());

    m_networkManager->post(request, message.toUtf8());
}


void CommunicationsSystem::sendTelemetryBurst() {
    sendNtfyNotification(
        "ZETA AVIONICS: Telemetry Burst",
        "Outbound telemetry package transmitted to Earth Relay successfully."
    );
    m_telemetryPackets += 15;
    emit telemetryPacketsChanged();
}


void CommunicationsSystem::triggerDistressBeacon() {
    sendNtfyNotification(
        "CRITICAL ALERT: Zeta Avionics",
        "Master Caution triggered. Emergency broadcast sequence active."
    );
}

void CommunicationsSystem::updateSimulation(float deltaTime) {
    float oldSignalStrength = m_signalStrength;
    float oldDataTransferRate = m_dataTransferRate;
    int oldTelemetryPackets = m_telemetryPackets;
    bool oldDownlinkStatus = m_downlinkStatus;
    QString oldAntennaTargetLock = m_antennaTargetLock;
    QString oldActiveLink = m_activeLink;

    if (m_downlinkStatus) {
        m_dataTransferRate = 128.5f;
    }
    else {
        m_dataTransferRate = 0.0f;
    }

    if (m_signalStrength != oldSignalStrength) emit signalStrengthChanged();
    if (m_dataTransferRate != oldDataTransferRate) emit dataTransferRateChanged();
    if (m_telemetryPackets != oldTelemetryPackets) emit telemetryPacketsChanged();
    if (m_downlinkStatus != oldDownlinkStatus) emit downlinkStatusChanged();
    if (m_antennaTargetLock != oldAntennaTargetLock) emit antennaTargetLockChanged();
    if (m_activeLink != oldActiveLink) emit activeLinkChanged();
}