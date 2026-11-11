#include "DiagnosticsSystem.h"

DiagnosticsSystem::DiagnosticsSystem(QObject *parent)
    : QObject(parent)
    , m_activeFaultCount(0)
    , m_systemHealthStatus("NOMINAL")
{
    m_errorLogs.append("[SYS_INIT] Flight avionics diagnostics initialized. Telemetry channels green.");
}

int DiagnosticsSystem::activeFaultCount() const { return m_activeFaultCount; }
QString DiagnosticsSystem::systemHealthStatus() const { return m_systemHealthStatus; }
QStringList DiagnosticsSystem::errorLogs() const { return m_errorLogs; }

void DiagnosticsSystem::runDiagnosticsScan() {
    m_errorLogs.prepend("[DIAG_SWEEP] Full subsystem telemetry poll complete. Cross-referencing telemetry nodes...");
    emit errorLogsChanged();
}

void DiagnosticsSystem::clearFaults() {
    m_errorLogs.clear();
    m_errorLogs.append("[SYS_LOG] Fault memory cleared by flight engineer override.");
    m_activeFaultCount = 0;
    m_systemHealthStatus = "NOMINAL";
    emit errorLogsChanged();
    emit activeFaultCountChanged();
    emit systemHealthStatusChanged();
}

void DiagnosticsSystem::updateSimulation(
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
) {
    int faults = 0;
    QStringList currentFaults;

    if (propPower <= 0.0f) { 
        faults++; 
        currentFaults.append("[ERR_BUS_PROP] Propulsion bus offline (0.0 kW)."); 
    } else if (propPower < 2.0f) { 
        faults++; 
        currentFaults.append(QString("[WARN_PWR_LOW] Propulsion power starved (%1 kW).").arg(propPower, 0, 'f', 1)); 
    } else if (propPower > 20.0f) { 
        faults++; 
        currentFaults.append(QString("[WARN_PWR_HIGH] Propulsion power overload risk (%1 kW).").arg(propPower, 0, 'f', 1)); 
    }

    if (lifePower <= 0.0f) { 
        faults++; 
        currentFaults.append("[ERR_BUS_LIFE] Life support bus offline (0.0 kW)."); 
    } else if (lifePower < 2.0f) { 
        faults++; 
        currentFaults.append(QString("[WARN_PWR_LOW] Life support power starved (%1 kW).").arg(lifePower, 0, 'f', 1)); 
    }

    if (navPower <= 0.0f)  { 
        faults++; 
        currentFaults.append("[ERR_BUS_NAV] Navigation computer bus offline (0.0 kW)."); 
    }

    if (commPower <= 0.0f) { 
        faults++; 
        currentFaults.append("[ERR_BUS_COMM] Communications array bus offline (0.0 kW)."); 
    }

    if (batteryBus <= 15.0f) {
        faults++;
        currentFaults.append(QString("[CRIT_BATTERY] Main battery bus critically low: %1%").arg(batteryBus, 0, 'f', 1));
    }

    if (commPower > 0.0f) {
        if (antennaLock != "LOCKED") {
            faults++;
            currentFaults.append(QString("[WARN_COMMS_ALIGN] Antenna alignment lost. Status: %1").arg(antennaLock));
        }
        if (commsSignal < 30.0f && commsSignal > 0.0f) {
            faults++;
            currentFaults.append(QString("[WARN_SIGNAL_WEAK] DSN signal strength degraded: %1%").arg(commsSignal, 0, 'f', 1));
        }
    }

    static int lastPacketCount = 0;
    if (telemetryPackets > lastPacketCount + 10) {
        currentFaults.append(QString("[INFO_TELEMETRY] Telemetry burst transmitted successfully. Total packets: %1").arg(telemetryPackets));
        lastPacketCount = telemetryPackets;
    }

    if (engineTemp >= 250.0f) {
        faults++;
        currentFaults.append(QString("[WARN_THERMAL] Propulsion core overheating: %1 C").arg(engineTemp, 0, 'f', 1));
    }
    if (!engineAvailable) {
        faults++;
        currentFaults.append("[FAIL_MAIN_ENGINE] Main engine lockout triggered (Thermal/Safety Interlock).");
    }
    if (propellant <= 5.0f) {
        faults++;
        currentFaults.append(QString("[CRIT_FUEL] Propellant reserves critical: %1%").arg(propellant, 0, 'f', 1));
    }

    if (cabinPressure < 80.0f || cabinPressure > 120.0f) {
        faults++;
        currentFaults.append(QString("[ERR_CABIN_PRESSURE] Abnormal cabin pressure anomaly: %1 kPa").arg(cabinPressure, 0, 'f', 1));
    }
    if (o2PartialPress < 15.0f) {
        faults++;
        currentFaults.append(QString("[CRIT_O2] Hypoxia risk: Low O2 partial pressure (%1 kPa)").arg(o2PartialPress, 0, 'f', 1));
    }
    if (co2Concentration > 1.0f) {
        faults++;
        currentFaults.append(QString("[WARN_CO2] Hypercapnia risk: High CO2 concentration (%1%)").arg(co2Concentration, 0, 'f', 2));
    }
    if (cabinTemp < 15.0f || cabinTemp > 30.0f) {
        faults++;
        currentFaults.append(QString("[WARN_CABIN_TEMP] Cabin thermal deviation: %1 C").arg(cabinTemp, 0, 'f', 1));
    }
    if (portableWater < 10.0f) {
        faults++;
        currentFaults.append(QString("[WARN_WATER] Potable water reserves low: %1%").arg(portableWater, 0, 'f', 1));
    }
    if (!liOHActive && lifePower > 0.0f) {
        faults++;
        currentFaults.append("[WARN_SCRUBBERS] LiOH CO2 scrubbers are offline while life support is powered.");
    }

    auto checkAstronautHealth = [&](int id, float temp, int bpm, int spo2, int resp) {
        if (spo2 > 0 && spo2 < 92) {
            faults++;
            currentFaults.append(QString("[MED_ASTRONAUT_%1] Hypoxia detected! SpO2 at %2%").arg(id).arg(spo2));
        }
        if (bpm > 140 || (bpm < 40 && bpm > 0)) {
            faults++;
            currentFaults.append(QString("[MED_ASTRONAUT_%1] Cardiac anomaly: Heart rate at %2 BPM").arg(id).arg(bpm));
        }
        if (temp > 39.0f || (temp < 35.0f && temp > 0.0f)) {
            faults++;
            currentFaults.append(QString("[MED_ASTRONAUT_%1] Core temperature variance: %2 C").arg(id).arg(temp, 0, 'f', 1));
        }
    };

    checkAstronautHealth(1, a1Temp, a1Bpm, a1Spo2, a1Resp);
    checkAstronautHealth(2, a2Temp, a2Bpm, a2Spo2, a2Resp);
    checkAstronautHealth(3, a3Temp, a3Bpm, a3Spo2, a3Resp);
    checkAstronautHealth(4, a4Temp, a4Bpm, a4Spo2, a4Resp);

    bool logsChanged = false;
    for (const QString &fault : currentFaults) {
        if (!m_errorLogs.contains(fault)) {
            m_errorLogs.prepend(fault);
            logsChanged = true;
        }
    }

    if (m_activeFaultCount != faults || logsChanged) {
        m_activeFaultCount = faults;
        emit activeFaultCountChanged();
        if (logsChanged) emit errorLogsChanged();
    }

    QString newHealth = "NOMINAL";
    if (faults > 0 && faults <= 2) {
        newHealth = "DEGRADED";
    } else if (faults > 2) {
        newHealth = "CRITICAL FAULT";
    }

    if (m_systemHealthStatus != newHealth) {
        m_systemHealthStatus = newHealth;
        emit systemHealthStatusChanged();
    }
}