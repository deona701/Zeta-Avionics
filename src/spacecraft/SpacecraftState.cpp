#include "SpacecraftState.h"

SpacecraftState::SpacecraftState(QObject *parent) : QObject(parent) {}

void SpacecraftState::updateAllSystems(float deltaTime)
{
    m_power.updateSimulation(deltaTime);
    
    m_propulsion.setPowerConsumption(m_power.propulsionPower());
    m_lifeSupport.setPowerConsumption(m_power.lifeSupportPower());
    m_navigation.setPowerConsumption(m_power.navigationPower());
    m_communications.setPowerConsumption(m_power.communicationsPower());

    m_propulsion.updateSimulation(deltaTime);
    m_communications.updateSimulation(deltaTime);
    m_lifeSupport.updateSimulation(deltaTime);

    float currentThrust = m_propulsion.thrustOutput();
    float currentFuel = m_propulsion.propellantPercentage();
    bool engineStatus = m_propulsion.engineStatus();

    float propulsionPowerConsumption = m_propulsion.powerConsumption();
    float lifeSupportPowerConsumption = m_lifeSupport.powerConsumption();
    float navigationPowerConsumption = m_navigation.powerConsumption();
    float communicationsPowerConsumption = m_communications.powerConsumption();

    m_navigation.updateSimulation(deltaTime, currentThrust, currentFuel, engineStatus);
}