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
    , m_antennaAlignment(91.7f)
    , m_telemetryPackets(0)
    , m_downlinkStatus(true)
    , m_antennaTargetLock("LOCKED")
    , m_activeLink("EARTH RELAY / DSN")
{
}

float CommunicationsSystem::signalStrength() const { return m_signalStrength; }
float CommunicationsSystem::dataTransferRate() const { return m_dataTransferRate; }
float CommunicationsSystem::antennaAlignment() const { return m_antennaAlignment; }
int CommunicationsSystem::telemetryPackets() const { return m_telemetryPackets; }
bool CommunicationsSystem::downlinkStatus() const { return m_downlinkStatus; }
QString CommunicationsSystem::antennaTargetLock() const { return m_antennaTargetLock; }
QString CommunicationsSystem::activeLink() const { return m_activeLink; }


void CommunicationsSystem::setAntennaAlignment(float newAntennaAlignment) {
    float clampedAntennaAlignment = std::clamp(newAntennaAlignment, 0.0f, 100.0f);

    if (m_antennaAlignment == clampedAntennaAlignment) {
        return;
    }

    m_antennaAlignment = clampedAntennaAlignment;
    emit antennaAlignmentChanged();
}


void CommunicationsSystem::setDownlinkStatus(bool newDownlinkStatus) {

    if (m_downlinkStatus == newDownlinkStatus) {
        return;
    }

    m_downlinkStatus = newDownlinkStatus;
    emit downlinkStatusChanged();
}


void CommunicationsSystem::setActiveLink(QString newActiveLink) {

    if (m_activeLink == newActiveLink) {
        return;
    }

    m_activeLink = newActiveLink;
    emit activeLinkChanged();
}


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
    float oldAntennaAlignment = m_antennaAlignment;
    int oldTelemetryPackets = m_telemetryPackets;
    bool oldDownlinkStatus = m_downlinkStatus;
    QString oldAntennaTargetLock = m_antennaTargetLock;
    QString oldActiveLink = m_activeLink;

    if (!m_downlinkStatus) {
        m_antennaTargetLock = "OFFLINE...";
        m_dataTransferRate = 0.0f;
        m_signalStrength = 0.0f;
    }
    else {
        float distanceToCenter = 0.0f;
        float maxAllowedDistance = 0.0f;

        if (m_activeLink == "EARTH RELAY / DSN") {
            float center = 93.0f;
            maxAllowedDistance = 7.0f;
            distanceToCenter = std::abs(m_antennaAlignment - center);

            if (m_antennaAlignment >= 86.0f) {
                m_antennaTargetLock = "LOCKED";
            }
            else {
                m_antennaTargetLock = "LOST...";
            }
        }
        else if (m_activeLink == "MARS ORBITER RELAY") {
            float center = 39.5f;
            maxAllowedDistance = 3.5f;
            distanceToCenter = std::abs(m_antennaAlignment - center);

            if (m_antennaAlignment >= 36.0f && m_antennaAlignment <= 43.0f) {
                m_antennaTargetLock = "LOCKED";
            }
            else {
                m_antennaTargetLock = "LOST...";
            }
        }
        else if (m_activeLink == "DEEP SPACE GATEWAY") {
            float center = 55.5f;
            maxAllowedDistance = 2.5f;
            distanceToCenter = std::abs(m_antennaAlignment - center);

            if (m_antennaAlignment >= 53.0f && m_antennaAlignment <= 58.0f) {
                m_antennaTargetLock = "LOCKED";
            }
            else {
                m_antennaTargetLock = "LOST...";
            }
        }

        if (distanceToCenter <= maxAllowedDistance) {
            float signalPercent = (1.0f - (distanceToCenter / maxAllowedDistance)) * 100.0f;
            m_signalStrength = std::clamp(signalPercent, 10.0f, 100.0f);

            m_dataTransferRate = (m_signalStrength / 100.0f) * 256.0f;
        }
        else {
            m_antennaTargetLock = "LOST...";
            m_signalStrength = 0.0f;
            m_dataTransferRate = 0.0f;
        }
    }

    if (m_signalStrength != oldSignalStrength) emit signalStrengthChanged();
    if (m_dataTransferRate != oldDataTransferRate) emit dataTransferRateChanged();
    if (m_antennaAlignment != oldAntennaAlignment) emit antennaAlignmentChanged();
    if (m_telemetryPackets != oldTelemetryPackets) emit telemetryPacketsChanged();
    if (m_downlinkStatus != oldDownlinkStatus) emit downlinkStatusChanged();
    if (m_antennaTargetLock != oldAntennaTargetLock) emit antennaTargetLockChanged();
    if (m_activeLink != oldActiveLink) emit activeLinkChanged();
}