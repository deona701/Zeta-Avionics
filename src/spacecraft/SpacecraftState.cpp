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

    m_diagnostics.updateSimulation(
        deltaTime,
        m_power.propulsionPower(), m_power.lifeSupportPower(), m_power.navigationPower(), m_power.communicationsPower(),
        m_power.mainBatteryBus(), m_power.inEclipse(),
        m_propulsion.engineTemp(), m_propulsion.propellantPercentage(), m_propulsion.mainEngineAvailability(),
        m_lifeSupport.cabinPressure(), m_lifeSupport.o2PartialPress(), m_lifeSupport.co2Concentration(),
        m_lifeSupport.cabinTemp(), m_lifeSupport.portableWater(), m_lifeSupport.liOHScrubbersPercent(), m_lifeSupport.liOHScrubbersActive(),
        m_communications.signalStrength(), m_communications.telemetryPackets(), m_communications.antennaTargetLock(),
        m_lifeSupport.astronaut1Temp(), m_lifeSupport.astronaut1BPM(), m_lifeSupport.astronaut1SPO2(), m_lifeSupport.astronaut1Resp(),
        m_lifeSupport.astronaut2Temp(), m_lifeSupport.astronaut2BPM(), m_lifeSupport.astronaut2SPO2(), m_lifeSupport.astronaut2Resp(),
        m_lifeSupport.astronaut3Temp(), m_lifeSupport.astronaut3BPM(), m_lifeSupport.astronaut3SPO2(), m_lifeSupport.astronaut3Resp(),
        m_lifeSupport.astronaut4Temp(), m_lifeSupport.astronaut4BPM(), m_lifeSupport.astronaut4SPO2(), m_lifeSupport.astronaut4Resp()
    );
    m_missions.updateSimulation(
        deltaTime,
        m_navigation.distance(),
        m_propulsion.propellantPercentage(),
        m_navigation.velocity()
    );
}