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