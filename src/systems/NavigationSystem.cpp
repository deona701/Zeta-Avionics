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

void NavigationSystem::updateSimulation(float deltaTime, float currentThrust, float currentFuel) {
    // Future code
}