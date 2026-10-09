#include "SpacecraftState.h"

SpacecraftState::SpacecraftState(QObject *parent) : QObject(parent) {}

void SpacecraftState::updateAllSystems(float deltaTime)
{
    m_propulsion.updateSimulation(deltaTime);
}