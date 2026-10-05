#ifndef PROPULSIONSYSTEM_H
#define PROPULSIONSYSTEM_H

#include <QObject>
#include <qqmlintegration.h>

class PropulsionSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

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

    static constexpr float GRAVITY = 9.80665f;
    static constexpr float DRY_MASS = 1000.0f;
    static constexpr float MAX_PROPELLANT_MASS = 2000.0f;
    static constexpr float ISP = 310.0f;

    static constexpr float MAX_THRUST = 500.0f;
    static constexpr float MAX_TEMP = 150.0f;
    static constexpr float MAX_PROPELLANT_BURN_RATE = 0.15f;
    static constexpr float TEMP_HEAT_RATE = 25.0f;
    static constexpr float TEMP_COOL_RATE = 0.15f;
    static constexpr float AMBIENT_TEMP = 20.0f;

    Q_INVOKABLE void updateSimulation(float deltaTime);

public slots:
    void setThrottle(float newThrottle);
    void setEngineStatus(bool newStatus);

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