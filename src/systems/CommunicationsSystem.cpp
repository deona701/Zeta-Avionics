#include "CommunicationsSystem.h"
#include "../spacecraft/SpacecraftState.h"

CommunicationsSystem::CommunicationsSystem(QObject *parent)
    : QObject(parent)
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


void CommunicationsSystem::updateSimulation(float deltaTime) {
    
}