#ifndef POWERSYSTEM_H
#define POWERSYSTEM_H

#include <QObject>
#include <qqmlintegration.h>

class PowerSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float totalGeneration READ totalGeneration NOTIFY totalGenerationChanged)
    Q_PROPERTY(float totalConsumption READ totalConsumption NOTIFY totalConsumptionChanged)
    Q_PROPERTY(float netBusBalance READ netBusBalance NOTIFY netBusBalanceChanged)
    Q_PROPERTY(float solarArrayOutput READ solarArrayOutput NOTIFY solarArrayOutputChanged)
    Q_PROPERTY(float reactorCoreOutput READ reactorCoreOutput NOTIFY reactorCoreOutputChanged)
    Q_PROPERTY(float mainBatteryBus READ mainBatteryBus NOTIFY mainBatteryBusChanged)
    Q_PROPERTY(float orbitTimer READ orbitTimer NOTIFY orbitTimerChanged)
    Q_PROPERTY(bool inEclipse READ inEclipse NOTIFY inEclipseChanged)

    Q_PROPERTY(float propulsionPower READ propulsionPower WRITE setPropulsionPower NOTIFY propulsionPowerChanged)
    Q_PROPERTY(float lifeSupportPower READ lifeSupportPower WRITE setLifeSupportPower NOTIFY lifeSupportPowerChanged)
    Q_PROPERTY(float navigationPower READ navigationPower WRITE setNavigationPower NOTIFY navigationPowerChanged)
    Q_PROPERTY(float communicationsPower READ communicationsPower WRITE setCommunicationsPower NOTIFY communicationsPowerChanged)

public:
    explicit PowerSystem(QObject *parent = nullptr);

    float totalGeneration() const;
    float totalConsumption() const;
    float netBusBalance() const;
    float solarArrayOutput() const;
    float reactorCoreOutput() const;
    float mainBatteryBus() const;
    float orbitTimer() const;
    bool inEclipse() const;

    float propulsionPower() const;
    float lifeSupportPower() const;
    float navigationPower() const;
    float communicationsPower() const;


    Q_INVOKABLE void updateSimulation(float deltaTime);
    Q_INVOKABLE void requestPowerReroute(const QString &systemId, float deltaKw);

public slots:
    void setPropulsionPower(float newPower);
    void setLifeSupportPower(float newPower);
    void setNavigationPower(float newPower);
    void setCommunicationsPower(float newPower);

signals:
    void totalGenerationChanged();
    void totalConsumptionChanged();
    void netBusBalanceChanged();
    void solarArrayOutputChanged();
    void reactorCoreOutputChanged();
    void mainBatteryBusChanged();
    void orbitTimerChanged();
    void inEclipseChanged();

    void propulsionPowerChanged();
    void lifeSupportPowerChanged();
    void navigationPowerChanged();
    void communicationsPowerChanged();

private:
    float m_solarArrayOutput = 15.0f;
    float m_reactorCoreOutput = 25.0f;
    float m_mainBatteryBus = 85.0f;

    float m_orbitTimer = 0.0f;
    bool m_inEclipse = false;

    float m_propulsionPower = 8.0f;
    float m_lifeSupportPower = 6.0f;
    float m_navigationPower = 4.0f;
    float m_communicationsPower = 3.0f;

    float m_totalGeneration = 15.0f + 25.0f;
    float m_totalConsumption = 8.0f + 6.0f + 4.0f + 3.0f;
    float m_netBusBalance = (15.0f + 25.0f) - (8.0f + 6.0f + 4.0f + 3.0f);
};

#endif // POWERSYSTEM_H