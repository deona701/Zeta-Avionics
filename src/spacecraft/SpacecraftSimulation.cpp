#include "SpacecraftSimulation.h"

SpacecraftSimulation::SpacecraftSimulation(QObject *parent) : QObject(parent)
{
    connect(&m_loopTimer, &QTimer::timeout, this, &SpacecraftSimulation::tick);
    m_loopTimer.setInterval(16);
}

void SpacecraftSimulation::setTimeMultiplier(float multiplier)
{
    if (multiplier > 0.0f) {
        m_timeMultiplier = multiplier;
    }
}

void SpacecraftSimulation::start()
{
    m_isRunning = true;
    m_frameTimer.start();
    m_loopTimer.start();
    emit isRunningChanged();
}

void SpacecraftSimulation::pause()
{
    m_isRunning = false;
    m_loopTimer.stop();
    emit isRunningChanged();
}

void SpacecraftSimulation::tick()
{
    float deltaTime = m_frameTimer.restart() / 1000.0f;

    float effectiveDelta = deltaTime * m_timeMultiplier;

    m_state.updateAllSystems(effectiveDelta);
}