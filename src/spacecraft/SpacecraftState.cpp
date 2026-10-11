#include "SpacecraftState.h"

SpacecraftState::SpacecraftState(QObject *parent) : QObject(parent) {}

void SpacecraftState::updateAllSystems(float deltaTime)
{
    m_propulsion.updateSimulation(deltaTime);

    float currentThrust = m_propulsion.thrustOutput();
    float currentFuel = m_propulsion.propellantPercentage();

    m_navigation.updateSimulation(deltaTime, currentThrust, currentFuel);
}