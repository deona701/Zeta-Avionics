#include "PropulsionSystem.h"
#include <algorithm>
#include <cmath>

PropulsionSystem::PropulsionSystem(QObject *parent)
    : QObject(parent)
    , m_throttle(0.0f)
    , m_engineTemp(20.0f)
    , m_propellantPercentage(100.0f)
    , m_deltaV(3340.0f)
    , m_thrustOutput(0.0f)
    , m_engineStatus(true)
    , m_mainEngineAvailability(true)
{
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
    float oldThrottle = m_throttle;
    float oldTemp = m_engineTemp;
    float oldPropellant = m_propellantPercentage;
    float oldDeltaV = m_deltaV;
    float oldThrust = m_thrustOutput;
    bool oldEngineStatus = m_engineStatus;
    bool oldAvailability = m_mainEngineAvailability;

    float throttleRatio = m_throttle / 100.0f;

    float fuelMass = (m_propellantPercentage / 100.0f) * MAX_PROPELLANT_MASS;
    float totalMass = DRY_MASS + fuelMass;

    float heatIn = 0.0f;
    if (m_engineStatus && m_mainEngineAvailability) {
        heatIn = (throttleRatio * throttleRatio) * TEMP_HEAT_RATE;
    }
    float heatOut = TEMP_COOL_RATE * (m_engineTemp - AMBIENT_TEMP);

    m_engineTemp += (heatIn - heatOut) * deltaTime;
    m_engineTemp = std::clamp(m_engineTemp, AMBIENT_TEMP, MAX_TEMP);

    if (m_engineTemp >= MAX_TEMP) {
        m_mainEngineAvailability = false;
        m_engineStatus = false;
    }
    else if (m_engineTemp < 80.0f && !m_mainEngineAvailability) {
        m_mainEngineAvailability = true;
    }

    if (m_propellantPercentage <= 0.0f) {
        m_engineStatus = false;
    }

    if (m_engineStatus && m_mainEngineAvailability) {
        m_thrustOutput = throttleRatio * MAX_THRUST;

        m_propellantPercentage -= throttleRatio * MAX_PROPELLANT_BURN_RATE * deltaTime;
        m_propellantPercentage = std::clamp(m_propellantPercentage, 0.0f, 100.0f);

        fuelMass = (m_propellantPercentage / 100.0f) * MAX_PROPELLANT_MASS;
        totalMass = DRY_MASS + fuelMass;
        m_deltaV = ISP * GRAVITY * std::log(totalMass / DRY_MASS);
    }
    else {
        m_thrustOutput = 0.0f;
    }

    if (m_throttle != oldThrottle) emit throttleChanged();
    if (m_engineTemp != oldTemp) emit engineTempChanged();
    if (m_propellantPercentage != oldPropellant) emit propellantPercentageChanged();
    if (m_deltaV != oldDeltaV) emit deltaVChanged();
    if (m_thrustOutput != oldThrust) emit thrustOutputChanged();
    if (m_engineStatus != oldEngineStatus) emit engineStatusChanged();
    if (m_mainEngineAvailability != oldAvailability) emit mainEngineAvailabilityChanged();
}