#ifndef DIAGNOSTICS_SYSTEM_H
#define DIAGNOSTICS_SYSTEM_H

#include <QObject>
#include <QStringList>
#include <qqmlintegration.h>

class DiagnosticsSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int activeFaultCount READ activeFaultCount NOTIFY activeFaultCountChanged)
    Q_PROPERTY(QString systemHealthStatus READ systemHealthStatus NOTIFY systemHealthStatusChanged)
    Q_PROPERTY(QStringList errorLogs READ errorLogs NOTIFY errorLogsChanged)

public:
    explicit DiagnosticsSystem(QObject *parent = nullptr);

    int activeFaultCount() const;
    QString systemHealthStatus() const;
    QStringList errorLogs() const;

    Q_INVOKABLE void runDiagnosticsScan();
    Q_INVOKABLE void clearFaults();

public slots:
    void updateSimulation(
        float deltaTime,
        float propPower, float lifePower, float navPower, float commPower,
        float batteryBus, bool inEclipse,
        float engineTemp, float propellant, bool engineAvailable,
        float cabinPressure, float o2PartialPress, float co2Concentration, 
        float cabinTemp, float portableWater, float liOHPercent, bool liOHActive,
        float commsSignal, int telemetryPackets, QString antennaLock,
        float a1Temp, int a1Bpm, int a1Spo2, int a1Resp,
        float a2Temp, int a2Bpm, int a2Spo2, int a2Resp,
        float a3Temp, int a3Bpm, int a3Spo2, int a3Resp,
        float a4Temp, int a4Bpm, int a4Spo2, int a4Resp
    );

signals:
    void activeFaultCountChanged();
    void systemHealthStatusChanged();
    void errorLogsChanged();

private:
    int m_activeFaultCount = 0;
    QString m_systemHealthStatus = "NOMINAL";
    QStringList m_errorLogs;
};

#endif // DIAGNOSTICS_SYSTEM_H