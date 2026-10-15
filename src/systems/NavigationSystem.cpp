#include "NavigationSystem.h"
#include "SpacecraftState.h"

NavigationSystem::NavigationSystem(QObject *parent)
    : QObject(parent)
    , m_distance(225000000.0f)
    , m_velocity(12.0f)
    , m_eta(217)
    , m_currentPos("Earth")
    , m_destination("Mars")
{
}

float NavigationSystem::distance() const { return m_distance; }
float NavigationSystem::velocity() const { return m_velocity; }
int NavigationSystem::eta() const { return m_eta; }
QString NavigationSystem::currentPos() const { return m_currentPos; }
QString NavigationSystem::destination() const { return m_destination; }
QString NavigationSystem::trajectory() const { return m_trajectory; }

void NavigationSystem::setDistance(float newDistance) {
    if (qFuzzyCompare(m_distance, newDistance)) {
        return;
    }
    m_distance = newDistance;
    emit distanceChanged();
}

void NavigationSystem::setVelocity(float newVelocity) {
    if (qFuzzyCompare(m_velocity, newVelocity)) {
        return;
    }
    m_velocity = newVelocity;
    emit velocityChanged();
}


void NavigationSystem::setEta(int newEta) {
    if (m_eta == newEta) {
        return;
    }
    m_eta = newEta;
    emit etaChanged();
}


void NavigationSystem::setCurrentPos(const QString &newPos) {
    if (m_currentPos == newPos) {
        return;
    }
    m_currentPos = newPos;
    emit currentPosChanged();
}

void NavigationSystem::setDestination(const QString &newDestination) {
    if (m_destination == newDestination) {
        return;
    }
    m_destination = newDestination;
    emit destinationChanged();
}

void NavigationSystem::setTrajectory(const QString &newTrajectory) {
    if (m_trajectory == newTrajectory) {
        return;
    }
    m_trajectory = newTrajectory;
    emit trajectoryChanged();
}

void NavigationSystem::updateSimulation(float deltaTime, float currentThrust, float currentFuel, bool engineStatus) {
    float oldDistance = m_distance;
    float oldVelocity = m_velocity;
    int oldEta = m_eta;
    QString oldCurrentPos = m_currentPos;
    QString oldDestination = m_destination;
    QString oldTrajectory = m_trajectory;

    float fuelMass = (currentFuel / 100.0f) * SpacecraftState::MAX_PROPELLANT_MASS;
    float totalMass = SpacecraftState::DRY_MASS + fuelMass;

    float acceleration =  currentThrust / totalMass;
    m_velocity += (acceleration / 1000.0f) * deltaTime;

    m_distance -= m_velocity * deltaTime;
    m_distance = std::clamp(m_distance, 0.0f, 225000000.0f);
    
    m_eta = (m_distance / m_velocity) / 86400.0;

    if (m_distance <= 0.0f) {
        m_currentPos = "Mars";
        m_destination = "Arrived";
    }

    if (currentFuel <= 0.0f) {
        m_trajectory = "Fuel Depleted (Coast)";
    }
    else if (currentThrust > 0.0f) {
        m_trajectory = "Powered Burn";
    }
    else {
        m_trajectory = "Nominal Coast";
    }

    if (m_distance != oldDistance) emit distanceChanged();
    if (m_velocity != oldVelocity) emit velocityChanged();
    if (m_eta != oldEta) emit etaChanged();
    if (m_currentPos != oldCurrentPos) emit currentPosChanged();
    if (m_destination != oldDestination) emit destinationChanged();
    if (m_trajectory != oldTrajectory) emit trajectoryChanged();
}