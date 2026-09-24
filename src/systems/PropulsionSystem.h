#ifndef PROPULSIONSYSTEM_H
#define PROPULSIONSYSTEM_H

#include <QObject>

class PropulsionSystem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float throttle READ throttle WRITE setThrottle NOTIFY throttleChanged)
    Q_PROPERTY(float engineTemp READ engineTemp NOTIFY engineTempChanged)
    Q_PROPERTY(float propellantPercentage READ propellantPercentage NOTIFY propellantPercentageChanged)
    Q_PROPERTY(float thrustOutput READ thrustOutput NOTIFY thrustOutputChanged)
    Q_PROPERTY(float deltaV READ deltaV NOTIFY deltaVChanged)
    Q_PROPERTY(bool engineStatus READ engineStatus WRITE setEngineStatus NOTIFY engineStatusChanged)
    Q_PROPERTY(bool mainEngineAvailability READ mainEngineAvailability NOTIFY mainEngineAvailabilityChanged)

public:
    explicit PropulsionSystem(QObject *parent = nullptr);

    float throttle() const;
    float engineTemp() const;
    float propellantPercentage() const;
    float deltaV() const;
    float thrustOutput() const;
    bool engineStatus() const;
    bool mainEngineAvailability() const;

    void setThrottle(float newThrottle);
    void setEngineStatus(bool newStatus);

    Q_INVOKABLE void updateSimulation(float deltaTime);

signals:
    void throttleChanged();
    void engineTempChanged();
    void propellantPercentageChanged();
    void deltaVChanged();
    void thrustOutputChanged();
    void engineStatusChanged();
    void mainEngineAvailabilityChanged();

private:
    float m_throttle = 0.0f;
    float m_engineTemp = 20.0f;
    float m_propellantPercentage = 100.0f;
    float m_deltaV = 2400.0f;
    float m_thrustOutput = 0.0f;
    bool m_engineStatus = false;
    bool m_mainEngineAvailability = true;
};

#endif // PROPULSIONSYSTEM_H