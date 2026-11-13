#include "MissionsSystem.h"
#include <algorithm>

MissionsSystem::MissionsSystem(QObject *parent)
    : QObject(parent)
    , m_powerConsumption(4.0f)
    , m_availableMissions({
        "Earth Departure",
        "Earth Orbit",
        "Lunar Transfer",
        "Deep Space Cruise",
        "Mars Approach",
        "Mars Orbit"
    })
{
}

float MissionsSystem::powerConsumption() const { return m_powerConsumption; }
QString MissionsSystem::currentMission() const { return m_currentMission; }
QString MissionsSystem::currentObjective() const { return m_currentObjective; }
float MissionsSystem::objectiveProgress() const { return m_objectiveProgress; }
QString MissionsSystem::missionStatus() const { return m_missionStatus; }
QStringList MissionsSystem::availableMissions() const { return m_availableMissions; }

void MissionsSystem::setPowerConsumption(float power) {
    if (m_powerConsumption == power) return;
    m_powerConsumption = power;
    emit powerConsumptionChanged();
}

void MissionsSystem::selectMission(const QString &missionName) {
    if (!m_availableMissions.contains(missionName)) return;

    m_currentMission = missionName;
    m_objectiveProgress = 0.0f;
    m_missionStatus = "IN PROGRESS";

    if (missionName == "Earth Departure") {
        m_currentObjective = "Execute Trans-Mars Injection (TMI) burn and clear Earth gravity well.";
    } else if (missionName == "Earth Orbit") {
        m_currentObjective = "Perform systems checkout, solar panel alignment, and bus synchronization in LEO.";
    } else if (missionName == "Lunar Transfer") {
        m_currentObjective = "Execute gravity assist trajectory via Lunar flyby and calibrate deep space DSN links.";
    } else if (missionName == "Deep Space Cruise") {
        m_currentObjective = "Maintain long-duration life support stability, radiation shielding, and thermal loops.";
    } else if (missionName == "Mars Approach") {
        m_currentObjective = "Align high-gain antenna with Mars Orbiter Relay and prep orbital insertion thrusters.";
    } else if (missionName == "Mars Orbit") {
        m_currentObjective = "Execute Mars Orbit Insertion (MOI) retro-burn and deploy atmospheric mapping telemetry.";
    }

    emit currentMissionChanged();
    emit currentObjectiveChanged();
    emit objectiveProgressChanged();
    emit missionStatusChanged();
}

void MissionsSystem::resetMission() {
    selectMission(m_currentMission);
}

void MissionsSystem::updateSimulation(float deltaTime, float navDistance, float propellant, float velocity) {
    if (m_powerConsumption <= 0.0f || m_missionStatus == "COMPLETED") {
        return;
    }

    if (m_currentMission == "Earth Departure") {
        m_objectiveProgress = std::clamp((velocity / 20.0f) * 100.0f, 0.0f, 100.0f);
    } 
    else if (m_currentMission == "Earth Orbit") {
        static float orbitTimer = 0.0f;
        orbitTimer += deltaTime;
        m_objectiveProgress = std::clamp((orbitTimer / 20.0f) * 100.0f, 0.0f, 100.0f);
    } 
    else if (m_currentMission == "Lunar Transfer") {
        float distanceCovered = 225000000.0f - navDistance;
        m_objectiveProgress = std::clamp((distanceCovered / 25000000.0f) * 100.0f, 0.0f, 100.0f);
    } 
    else if (m_currentMission == "Deep Space Cruise") {
        float cruiseProgress = ((200000000.0f - navDistance) / 150000000.0f) * 100.0f;
        m_objectiveProgress = std::clamp(cruiseProgress, 0.0f, 100.0f);
    } 
    else if (m_currentMission == "Mars Approach") {
        float approachProgress = ((50000000.0f - navDistance) / 45000000.0f) * 100.0f;
        m_objectiveProgress = std::clamp(approachProgress, 0.0f, 100.0f);
    } 
    else if (m_currentMission == "Mars Orbit") {
        float insertionProgress = ((5000000.0f - navDistance) / 5000000.0f) * 100.0f;
        m_objectiveProgress = std::clamp(insertionProgress, 0.0f, 100.0f);
    }

    if (m_objectiveProgress >= 100.0f) {
        m_objectiveProgress = 100.0f;
        m_missionStatus = "COMPLETED";
        emit missionStatusChanged();
    }

    emit objectiveProgressChanged();
}