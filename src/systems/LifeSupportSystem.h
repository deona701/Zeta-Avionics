#ifndef LIFESUPPORTSYSTEM_H
#define LIFESUPPORTSYSTEM_H

#include <QObject>
#include <QString>
#include <qqmlintegration.h>
#include <QHttpServer>

class LifeSupportSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float cabinPressure READ cabinPressure NOTIFY cabinPressureChanged)
    Q_PROPERTY(float o2PartialPress READ o2PartialPress NOTIFY o2PartialPressChanged)
    Q_PROPERTY(float co2Concentration READ co2Concentration NOTIFY co2ConcentrationChanged)
    Q_PROPERTY(float relativeHumidity READ relativeHumidity NOTIFY relativeHumidityChanged)
    Q_PROPERTY(float cabinTemp READ cabinTemp NOTIFY cabinTempChanged)
    Q_PROPERTY(float portableWater READ portableWater NOTIFY portableWaterChanged)
    Q_PROPERTY(float greywaterTank READ greywaterTank NOTIFY greywaterTankChanged)
    Q_PROPERTY(float wpaEfficiency READ wpaEfficiency NOTIFY wpaEfficiencyChanged)
    Q_PROPERTY(float liOHScrubbersPercent READ liOHScrubbersPercent NOTIFY liOHScrubbersPercentChanged)
    Q_PROPERTY(float coolantLoopA READ coolantLoopA NOTIFY coolantLoopAChanged)
    Q_PROPERTY(bool liOHScrubbersActive READ liOHScrubbersActive NOTIFY liOHScrubbersActiveChanged)

    Q_PROPERTY(float astronaut1Temp READ astronaut1Temp NOTIFY astronaut1TempChanged)
    Q_PROPERTY(int astronaut1BPM READ astronaut1BPM NOTIFY astronaut1BPMChanged)
    Q_PROPERTY(int astronaut1SPO2 READ astronaut1SPO2 NOTIFY astronaut1SPO2Changed)
    Q_PROPERTY(int astronaut1Resp READ astronaut1Resp NOTIFY astronaut1RespChanged)

    Q_PROPERTY(float astronaut2Temp READ astronaut2Temp NOTIFY astronaut2TempChanged)
    Q_PROPERTY(int astronaut2BPM READ astronaut2BPM NOTIFY astronaut2BPMChanged)
    Q_PROPERTY(int astronaut2SPO2 READ astronaut2SPO2 NOTIFY astronaut2SPO2Changed)
    Q_PROPERTY(int astronaut2Resp READ astronaut2Resp NOTIFY astronaut2RespChanged)

    Q_PROPERTY(float astronaut3Temp READ astronaut3Temp NOTIFY astronaut3TempChanged)
    Q_PROPERTY(int astronaut3BPM READ astronaut3BPM NOTIFY astronaut3BPMChanged)
    Q_PROPERTY(int astronaut3SPO2 READ astronaut3SPO2 NOTIFY astronaut3SPO2Changed)
    Q_PROPERTY(int astronaut3Resp READ astronaut3Resp NOTIFY astronaut3RespChanged)

    Q_PROPERTY(float astronaut4Temp READ astronaut4Temp NOTIFY astronaut4TempChanged)
    Q_PROPERTY(int astronaut4BPM READ astronaut4BPM NOTIFY astronaut4BPMChanged)
    Q_PROPERTY(int astronaut4SPO2 READ astronaut4SPO2 NOTIFY astronaut4SPO2Changed)
    Q_PROPERTY(int astronaut4Resp READ astronaut4Resp NOTIFY astronaut4RespChanged)

public:
    explicit LifeSupportSystem(QObject *parent = nullptr);

    float cabinPressure() const;
    float o2PartialPress() const;
    float co2Concentration() const;
    float relativeHumidity() const;
    float cabinTemp() const;
    float portableWater() const;
    float greywaterTank() const;
    float wpaEfficiency() const;
    float liOHScrubbersPercent() const;
    float coolantLoopA() const;
    bool liOHScrubbersActive() const;

    float astronaut1Temp() const;
    int astronaut1BPM() const;
    int astronaut1SPO2() const;
    int astronaut1Resp() const;

    float astronaut2Temp() const;
    int astronaut2BPM() const;
    int astronaut2SPO2() const;
    int astronaut2Resp() const;

    float astronaut3Temp() const;
    int astronaut3BPM() const;
    int astronaut3SPO2() const;
    int astronaut3Resp() const;

    float astronaut4Temp() const;
    int astronaut4BPM() const;
    int astronaut4SPO2() const;
    int astronaut4Resp() const;

    Q_INVOKABLE void updateSimulation(float deltaTime);

signals:
    void cabinPressureChanged();
    void o2PartialPressChanged();
    void co2ConcentrationChanged();
    void relativeHumidityChanged();
    void cabinTempChanged();
    void portableWaterChanged();
    void greywaterTankChanged();
    void wpaEfficiencyChanged();
    void liOHScrubbersPercentChanged();
    void coolantLoopAChanged();
    void liOHScrubbersActiveChanged();

    void astronaut1TempChanged();
    void astronaut1BPMChanged();
    void astronaut1SPO2Changed();
    void astronaut1RespChanged();

    void astronaut2TempChanged();
    void astronaut2BPMChanged();
    void astronaut2SPO2Changed();
    void astronaut2RespChanged();

    void astronaut3TempChanged();
    void astronaut3BPMChanged();
    void astronaut3SPO2Changed();
    void astronaut3RespChanged();

    void astronaut4TempChanged();
    void astronaut4BPMChanged();
    void astronaut4SPO2Changed();
    void astronaut4RespChanged();

private:
    QHttpServer *m_server;

    float m_cabinPressure = 101.3f;
    float m_o2PartialPress = 21.1f;
    float m_co2Concentration = 0.31f;
    float m_relativeHumidity = 45.0f;
    float m_cabinTemp = 21.5f;
    float m_portableWater = 88.4f;
    float m_greywaterTank = 32.1f;
    float m_wpaEfficiency = 94.8f;
    float m_liOHScrubbersPercent = 92.0f;
    float m_coolantLoopA = 4.2f;
    bool m_liOHScrubbersActive = false;

    float m_astronaut1Temp = 36.8f;
    int m_astronaut1BPM = 72;
    int m_astronaut1SPO2 = 98;
    int m_astronaut1Resp = 12;

    float m_astronaut2Temp = 36.8f;
    int m_astronaut2BPM = 72;
    int m_astronaut2SPO2 = 98;
    int m_astronaut2Resp = 12;

    float m_astronaut3Temp = 36.8f;
    int m_astronaut3BPM = 72;
    int m_astronaut3SPO2 = 98;
    int m_astronaut3Resp = 12;

    float m_astronaut4Temp = 36.8f;
    int m_astronaut4BPM = 72;
    int m_astronaut4SPO2 = 98;
    int m_astronaut4Resp = 12;
};

#endif // LIFESUPPORTSYSTEM_H
