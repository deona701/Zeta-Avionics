#include "NavigationSystem.h"
#include "SpacecraftState.h"
#include <algorithm>
#include <cmath>

NavigationSystem::NavigationSystem(QObject *parent)
    : QObject(parent)
    , m_powerConsumption(4.0f)
    , m_distance(225000000.0f)
    , m_velocity(24.5f)
    , m_eta(106)
    , m_currentPos("Earth Orbit")
    , m_destination("Mars Intercept")
    , m_trajectory("Nominal Interplanetary Transfer")
{
}

float NavigationSystem::powerConsumption() const { return m_powerConsumption; }
float NavigationSystem::distance() const { return m_distance; }
float NavigationSystem::velocity() const { return m_velocity; }
int NavigationSystem::eta() const { return m_eta; }
QString NavigationSystem::currentPos() const { return m_currentPos; }
QString NavigationSystem::destination() const { return m_destination; }
QString NavigationSystem::trajectory() const { return m_trajectory; }

void NavigationSystem::setPowerConsumption(float newPower) {
    if (m_powerConsumption == newPower) return;
    m_powerConsumption = newPower;
    emit powerConsumptionChanged();
}

void NavigationSystem::updateSimulation(float deltaTime, float currentThrust, float currentFuel, bool engineStatus) {
    float oldDistance = m_distance;
    float oldVelocity = m_velocity;
    int oldEta = m_eta;
    QString oldCurrentPos = m_currentPos;
    QString oldDestination = m_destination;
    QString oldTrajectory = m_trajectory;

    if (m_powerConsumption <= 0.0f) {
        return;
    }

    float nominalPower = 4.0f;
    float powerFactor = std::clamp(m_powerConsumption / nominalPower, 0.0f, 1.0f);

    float fuelMass = (currentFuel / 100.0f) * SpacecraftState::MAX_PROPELLANT_MASS;
    float totalMass = SpacecraftState::DRY_MASS + fuelMass;

    float rawAcceleration = (totalMass > 0.0f) ? (currentThrust / totalMass) : 0.0f;
    float accelerationKm = (rawAcceleration * 50.0f) * powerFactor;

    if (currentFuel > 0.0f && engineStatus && currentThrust > 0.0f) {
        m_velocity += accelerationKm * deltaTime;
        m_trajectory = "Powered Burn Vector";
    } 
    else {
        m_trajectory = "Heliocentric Coast";
    }

    m_velocity = std::clamp(m_velocity, 10.0f, 75.0f);

    float distanceCoveredKm = m_velocity * deltaTime * 150.0f;
    m_distance -= distanceCoveredKm;
    m_distance = std::clamp(m_distance, 0.0f, 225000000.0f);

    if (m_distance > 180000000.0f) {
        m_currentPos = "Earth Departure Orbit";
        m_destination = "Deep Space Transit";
    } 
    else if (m_distance > 50000000.0f) {
        m_currentPos = "Deep Space Cruise";
        m_destination = "Mars Approach Corridor";
    } 
    else if (m_distance > 5000000.0f) {
        m_currentPos = "Mars Approach";
        m_destination = "Mars Orbit Insertion (MOI)";
    } 
    else if (m_distance > 0.0f) {
        m_currentPos = "Mars Terminal Insertion";
        m_destination = "Low Mars Orbit (LMO)";
    } 
    else {
        m_currentPos = "Mars Surface / Orbit";
        m_destination = "Mission Accomplished";
        m_trajectory = "Stable Martian Capture";
    }

    if (m_velocity > 0.0f) {
        m_eta = static_cast<int>((m_distance / m_velocity) / 86400.0f);
    }

    if (currentFuel <= 0.0f) {
        m_trajectory = "Fuel Depleted - Ballistic Coast";
    }

    if (m_distance != oldDistance) emit distanceChanged();
    if (m_velocity != oldVelocity) emit velocityChanged();
    if (m_eta != oldEta) emit etaChanged();
    if (m_currentPos != oldCurrentPos) emit currentPosChanged();
    if (m_destination != oldDestination) emit destinationChanged();
    if (m_trajectory != oldTrajectory) emit trajectoryChanged();
}