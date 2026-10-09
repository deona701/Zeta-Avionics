#include "NavigationSystem.h"

NavigationSystem::NavigationSystem(QObject *parent)
    : QObject(parent)
    , m_distance(225000000.0f)
    , m_currentPos("Earth")
    , m_destination("Mars")
{
}

float NavigationSystem::distance() const { return m_distance; }
QString NavigationSystem::currentPos() const { return m_currentPos; }
QString NavigationSystem::destination() const { return m_destination; }

void NavigationSystem::setDistance(float newDistance) {
    if (qFuzzyCompare(m_distance, newDistance)) {
        return;
    }
    m_distance = newDistance;
    emit distanceChanged();
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

void NavigationSystem::updateSimulation(float deltaTime) {
    // Future code
}