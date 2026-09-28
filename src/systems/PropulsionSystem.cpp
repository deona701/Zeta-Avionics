#include "PropulsionSystem.h"
#include <algorithm>

PropulsionSystem::PropulsionSystem(QObject *parent) : QObject(parent){

}

float PropulsionSystem::throttle() const { return m_throttle; }
float PropulsionSystem::engineTemp() const { return m_engineTemp; }
float PropulsionSystem::propellantPercentage() const { return m_propellantPercentage; }
float PropulsionSystem::deltaV() const { return m_deltaV; }
float PropulsionSystem::thrustOutput() const { return m_thrustOutput; }
bool PropulsionSystem::engineStatus() const { return m_engineStatus; }
bool PropulsionSystem::mainEngineAvailability() const { return m_mainEngineAvailability; }


void PropulsionSystem::setThrottle(float newThrottle) {
    float clampedThrottle = std::clamp(newThrottle, 0.0f, 100.0f);

    if (m_throttle == clampedThrottle) {
        return;
    }

    m_throttle = clampedThrottle;
    emit throttleChanged();
}

void PropulsionSystem::setEngineStatus(bool newEngineStatus) {
    if (newEngineStatus == true) {
        if (!m_mainEngineAvailability || m_propellantPercentage <= 0.0f) {
            return;
        }
    }

    if (m_engineStatus == newEngineStatus) {
        return;
    }

    m_engineStatus = newEngineStatus;
    emit engineStatusChanged();
}

void PropulsionSystem::updateSimulation(float deltaTime) {
    float oldTemp = m_engineTemp;
    float oldPropellant = m_propellantPercentage;
    float oldDeltaV = m_deltaV;
    float oldThrust = m_thrustOutput;
    bool oldEngineStatus = m_engineStatus;
    bool oldAvailability = m_mainEngineAvailability;

    const float maxThrust = 500.0f;
    const float maxTemp = 150.0f;
    float maxPropellantBurnRate = 0.5f;
    float tempHeatRate = 12.0f;

    if (m_engineTemp >= maxTemp) {
        m_mainEngineAvailability = false;
        m_engineStatus = false;
    }

    if (m_propellantPercentage <= 0.0f) {
        m_engineStatus = false;
        m_thrustOutput = 0.0f;
    }

    if (m_engineStatus && m_mainEngineAvailability) {
        m_thrustOutput = (m_throttle / 100.0f) * maxThrust;

        m_propellantPercentage -= (m_throttle / 100.0f) * maxPropellantBurnRate * deltaTime;
        m_propellantPercentage = std::clamp(m_propellantPercentage, 0.0f, 100.0f);

        m_deltaV = (m_propellantPercentage / 100.0f) * 2400.0f;

        m_engineTemp += (m_throttle / 100.0f) * tempHeatRate * deltaTime;
    }

    else {
        m_thrustOutput = 0.0f;

        m_engineTemp -= 4.0f * deltaTime;
        m_engineTemp = std::clamp(m_engineTemp, 20.0f, maxTemp);
    }

    if (m_engineTemp != oldTemp) emit engineTempChanged();
    if (m_propellantPercentage != oldPropellant) emit propellantPercentageChanged();
    if (m_deltaV != oldDeltaV) emit deltaVChanged();
    if (m_thrustOutput != oldThrust) emit thrustOutputChanged();
    if (m_engineStatus != oldEngineStatus) emit engineStatusChanged();
    if (m_mainEngineAvailability != oldAvailability) emit mainEngineAvailabilityChanged();
}