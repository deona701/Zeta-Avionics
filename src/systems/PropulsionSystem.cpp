#include "PropulsionSystem.h"

PropulsionSystem::PropulsionSystem(QObject *parent) : QObject(parent){

}

float PropulsionSystem::throttle() const { return m_throttle; }
float PropulsionSystem::engineTemp() const { return m_engineTemp; }
float PropulsionSystem::propellantPercentage() const { return m_propellantPercentage; }
float PropulsionSystem::deltaV() const { return m_deltaV; }
float PropulsionSystem::thrustOutput() const { return m_thrustOutput; }
bool PropulsionSystem::engineStatus() const { return m_engineStatus; }
bool PropulsionSystem::mainEngineAvailability() const { return m_mainEngineAvailability; }
