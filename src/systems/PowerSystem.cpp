#include "PowerSystem.h"
#include <algorithm>

PowerSystem::PowerSystem(QObject *parent) 
    : QObject(parent)
    ,  m_solarArrayOutput(15.0f)
    , m_reactorCoreOutput(25.0f)
    , m_mainBatteryBus(85.0f)

    , m_propulsionPower(8.0f)
    , m_lifeSupportPower(6.0f)
    , m_navigationPower(4.0f)
    , m_communicationsPower(3.0f)

    , m_totalGeneration(15.0f + 25.0f)
    , m_totalConsumption(8.0f + 6.0f + 4.0f + 3.0f)
    , m_netBusBalance((15.0f + 25.0f) - (8.0f + 6.0f + 4.0f + 3.0f))
{
}

float PowerSystem::solarArrayOutput() const { return m_solarArrayOutput; }
float PowerSystem::reactorCoreOutput() const { return m_reactorCoreOutput; }
float PowerSystem::mainBatteryBus() const { return m_mainBatteryBus; }

float PowerSystem::propulsionPower() const { return m_propulsionPower; }
float PowerSystem::lifeSupportPower() const { return m_lifeSupportPower; }
float PowerSystem::navigationPower() const { return m_navigationPower; }
float PowerSystem::communicationsPower() const { return m_communicationsPower; }

float PowerSystem::totalGeneration() const { return m_totalGeneration; }
float PowerSystem::totalConsumption() const { return m_totalConsumption; }
float PowerSystem::netBusBalance() const { return m_netBusBalance; }


void PowerSystem::setPropulsionPower(float newPropulsionPower) {
    if (m_propulsionPower == newPropulsionPower) {
        return;
    }

    m_propulsionPower = newPropulsionPower;
    emit propulsionPowerChanged();
}


void PowerSystem::setLifeSupportPower(float newLifeSupportPower) {
    if (m_lifeSupportPower == newLifeSupportPower) {
        return;
    }

    m_lifeSupportPower = newLifeSupportPower;
    emit lifeSupportPowerChanged();
}


void PowerSystem::setNavigationPower(float newNavigationPower) {
    if (m_navigationPower == newNavigationPower) {
        return;
    }

    m_navigationPower = newNavigationPower;
    emit navigationPowerChanged();
}


void PowerSystem::setCommunicationsPower(float newCommunicationsPower) {
    if (m_communicationsPower == newCommunicationsPower) {
        return;
    }

    m_communicationsPower = newCommunicationsPower;
    emit communicationsPowerChanged();
}

void PowerSystem::requestPowerReroute(const QString &systemId, float deltaKw) {
    if (systemId == "propulsion") {
        setPropulsionPower(std::clamp(m_propulsionPower + deltaKw, 0.0f, 30.0f));
    } 
    else if (systemId == "life_support") {
        setLifeSupportPower(std::clamp(m_lifeSupportPower + deltaKw, 0.0f, 30.0f));
    } 
    else if (systemId == "navigation") {
        setNavigationPower(std::clamp(m_navigationPower + deltaKw, 0.0f, 30.0f));
    } 
    else if (systemId == "comms") {
        setCommunicationsPower(std::clamp(m_communicationsPower + deltaKw, 0.0f, 30.0f));
    }
}

void PowerSystem::updateSimulation(float deltaTime) {
    float oldSolarArrayOutput = m_solarArrayOutput;
    float oldReactorCoreOutput = m_reactorCoreOutput;
    float oldMainBatteryBus = m_mainBatteryBus;
    float oldTotalGeneration = m_totalGeneration;
    float oldTotalConsumption = m_totalConsumption;
    float oldNetBusBalance = m_netBusBalance;

    m_totalGeneration = m_solarArrayOutput + m_reactorCoreOutput;
    m_totalConsumption = m_propulsionPower + m_lifeSupportPower + m_navigationPower + m_communicationsPower;
    m_netBusBalance = m_totalGeneration - m_totalConsumption;

    m_mainBatteryBus += (m_netBusBalance * 0.01f) * deltaTime;
    m_mainBatteryBus = std::clamp(m_mainBatteryBus, 0.0f, 100.0f);

    if (m_mainBatteryBus <= 0.0f && m_netBusBalance < 0.0f) {
        m_propulsionPower = 0.0f;
        m_navigationPower = 0.0f;
        m_communicationsPower = 0.0f;
    }

    if (m_solarArrayOutput != oldSolarArrayOutput) emit solarArrayOutputChanged();
    if (m_reactorCoreOutput != oldReactorCoreOutput) emit reactorCoreOutputChanged();
    if (m_mainBatteryBus != oldMainBatteryBus) emit mainBatteryBusChanged();
    if (m_totalGeneration != oldTotalGeneration) emit totalGenerationChanged();
    if (m_totalConsumption != oldTotalConsumption) emit totalConsumptionChanged();
    if (m_netBusBalance != oldNetBusBalance) emit netBusBalanceChanged();
}