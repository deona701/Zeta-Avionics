#include "LifeSupportSystem.h"
#include <algorithm>
#include <QHostAddress>
#include <QTcpServer>
#include <QFile>

LifeSupportSystem::LifeSupportSystem(QObject *parent) 
: QObject(parent)
, m_cabinPressure(101.3f), m_o2PartialPress(21.1f), m_co2Concentration(0.31f), m_relativeHumidity(45.0f)
, m_cabinTemp(21.5f), m_portableWater(88.4f), m_greywaterTank(32.1f), m_wpaEfficiency(94.8f), m_liOHScrubbersPercent(92.0f)
, m_coolantLoopA(4.2f), m_liOHScrubbersActive(false)
, m_astronaut1Temp(36.8f), m_astronaut1BPM(72), m_astronaut1SPO2(98), m_astronaut1Resp(12)
, m_astronaut2Temp(36.8f), m_astronaut2BPM(72), m_astronaut2SPO2(98), m_astronaut2Resp(12)
, m_astronaut3Temp(36.8f), m_astronaut3BPM(72), m_astronaut3SPO2(98), m_astronaut3Resp(12)
, m_astronaut4Temp(36.8f), m_astronaut4BPM(72), m_astronaut4SPO2(98), m_astronaut4Resp(12)
, m_server(new QHttpServer(this))
{
    auto tcpServer = new QTcpServer(this);
    if (tcpServer->listen(QHostAddress::Any, 8080)) {
        m_server->bind(tcpServer);
    }

    auto serveHtml = []() {
        QFile file(":/qt/qml/Zeta_Avionics/assets/web/astronaut_control.html");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return QHttpServerResponse("text/html", file.readAll());
        }
        return QHttpServerResponse("text/html", QByteArray("<html><body><h3>Error loading control panel.</h3></body></html>"));
    };

    m_server->route("/astronaut/1", serveHtml);
    m_server->route("/astronaut/2", serveHtml);
    m_server->route("/astronaut/3", serveHtml);
    m_server->route("/astronaut/4", serveHtml);

    auto updateVal = [](int &val, int delta, int minVal, int maxVal) {
        val = std::clamp(val + delta, minVal, maxVal);
    };

    auto updateFloatVal = [](float &val, float delta, float minVal, float maxVal) {
        val = std::clamp(val + delta, minVal, maxVal);
    };

    m_server->route("/astronaut/1/increaseHeartRate", [this, updateVal]() { updateVal(m_astronaut1BPM, 1, 30, 220); emit astronaut1BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseHeartRate", [this, updateVal]() { updateVal(m_astronaut1BPM, -1, 30, 220); emit astronaut1BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/increaseSPO2",      [this, updateVal]() { updateVal(m_astronaut1SPO2, 1, 0, 100); emit astronaut1SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseSPO2",      [this, updateVal]() { updateVal(m_astronaut1SPO2, -1, 0, 100); emit astronaut1SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/increaseRespRate",  [this, updateVal]() { updateVal(m_astronaut1Resp, 1, 0, 60); emit astronaut1RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseRespRate",  [this, updateVal]() { updateVal(m_astronaut1Resp, -1, 0, 60); emit astronaut1RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/increaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut1Temp, 0.1f, 30.0f, 45.0f); emit astronaut1TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut1Temp, -0.1f, 30.0f, 45.0f); emit astronaut1TempChanged(); return QHttpServerResponse("text/plain", "OK"); });

    m_server->route("/astronaut/2/increaseHeartRate", [this, updateVal]() { updateVal(m_astronaut2BPM, 1, 30, 220); emit astronaut2BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseHeartRate", [this, updateVal]() { updateVal(m_astronaut2BPM, -1, 30, 220); emit astronaut2BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/increaseSPO2",      [this, updateVal]() { updateVal(m_astronaut2SPO2, 1, 0, 100); emit astronaut2SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseSPO2",      [this, updateVal]() { updateVal(m_astronaut2SPO2, -1, 0, 100); emit astronaut2SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/increaseRespRate",  [this, updateVal]() { updateVal(m_astronaut2Resp, 1, 0, 60); emit astronaut2RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseRespRate",  [this, updateVal]() { updateVal(m_astronaut2Resp, -1, 0, 60); emit astronaut2RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/increaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut2Temp, 0.1f, 30.0f, 45.0f); emit astronaut2TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut2Temp, -0.1f, 30.0f, 45.0f); emit astronaut2TempChanged(); return QHttpServerResponse("text/plain", "OK"); });

    m_server->route("/astronaut/3/increaseHeartRate", [this, updateVal]() { updateVal(m_astronaut3BPM, 1, 30, 220); emit astronaut3BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseHeartRate", [this, updateVal]() { updateVal(m_astronaut3BPM, -1, 30, 220); emit astronaut3BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/increaseSPO2",      [this, updateVal]() { updateVal(m_astronaut3SPO2, 1, 0, 100); emit astronaut3SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseSPO2",      [this, updateVal]() { updateVal(m_astronaut3SPO2, -1, 0, 100); emit astronaut3SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/increaseRespRate",  [this, updateVal]() { updateVal(m_astronaut3Resp, 1, 0, 60); emit astronaut3RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseRespRate",  [this, updateVal]() { updateVal(m_astronaut3Resp, -1, 0, 60); emit astronaut3RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/increaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut3Temp, 0.1f, 30.0f, 45.0f); emit astronaut3TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut3Temp, -0.1f, 30.0f, 45.0f); emit astronaut3TempChanged(); return QHttpServerResponse("text/plain", "OK"); });

    m_server->route("/astronaut/4/increaseHeartRate", [this, updateVal]() { updateVal(m_astronaut4BPM, 1, 30, 220); emit astronaut4BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseHeartRate", [this, updateVal]() { updateVal(m_astronaut4BPM, -1, 30, 220); emit astronaut4BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/increaseSPO2",      [this, updateVal]() { updateVal(m_astronaut4SPO2, 1, 0, 100); emit astronaut4SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseSPO2",      [this, updateVal]() { updateVal(m_astronaut4SPO2, -1, 0, 100); emit astronaut4SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/increaseRespRate",  [this, updateVal]() { updateVal(m_astronaut4Resp, 1, 0, 60); emit astronaut4RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseRespRate",  [this, updateVal]() { updateVal(m_astronaut4Resp, -1, 0, 60); emit astronaut4RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/increaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut4Temp, 0.1f, 30.0f, 45.0f); emit astronaut4TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseTemp",      [this, updateFloatVal]() { updateFloatVal(m_astronaut4Temp, -0.1f, 30.0f, 45.0f); emit astronaut4TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
}

float LifeSupportSystem::cabinPressure() const { return m_cabinPressure; }
float LifeSupportSystem::o2PartialPress() const { return m_o2PartialPress; }
float LifeSupportSystem::co2Concentration() const { return m_co2Concentration; }
float LifeSupportSystem::relativeHumidity() const { return m_relativeHumidity; }
float LifeSupportSystem::cabinTemp() const { return m_cabinTemp; }
float LifeSupportSystem::portableWater() const { return m_portableWater; }
float LifeSupportSystem::greywaterTank() const { return m_greywaterTank; }
float LifeSupportSystem::wpaEfficiency() const { return m_wpaEfficiency; }
float LifeSupportSystem::liOHScrubbersPercent() const { return m_liOHScrubbersPercent; }
float LifeSupportSystem::coolantLoopA() const { return m_coolantLoopA; }
bool LifeSupportSystem::liOHScrubbersActive() const { return m_liOHScrubbersActive; }

float LifeSupportSystem::astronaut1Temp() const { return m_astronaut1Temp; }
int LifeSupportSystem::astronaut1BPM() const { return m_astronaut1BPM; }
int LifeSupportSystem::astronaut1SPO2() const { return m_astronaut1SPO2; }
int LifeSupportSystem::astronaut1Resp() const { return m_astronaut1Resp; }

float LifeSupportSystem::astronaut2Temp() const { return m_astronaut2Temp; }
int LifeSupportSystem::astronaut2BPM() const { return m_astronaut2BPM; }
int LifeSupportSystem::astronaut2SPO2() const { return m_astronaut2SPO2; }
int LifeSupportSystem::astronaut2Resp() const { return m_astronaut2Resp; }

float LifeSupportSystem::astronaut3Temp() const { return m_astronaut3Temp; }
int LifeSupportSystem::astronaut3BPM() const { return m_astronaut3BPM; }
int LifeSupportSystem::astronaut3SPO2() const { return m_astronaut3SPO2; }
int LifeSupportSystem::astronaut3Resp() const { return m_astronaut3Resp; }

float LifeSupportSystem::astronaut4Temp() const { return m_astronaut4Temp; }
int LifeSupportSystem::astronaut4BPM() const { return m_astronaut4BPM; }
int LifeSupportSystem::astronaut4SPO2() const { return m_astronaut4SPO2; }
int LifeSupportSystem::astronaut4Resp() const { return m_astronaut4Resp; }

void LifeSupportSystem::updateSimulation(float deltaTime) {
    float oldCabinPressure = m_cabinPressure;
    float oldO2PartialPress = m_o2PartialPress;
    float oldCO2Concentration = m_co2Concentration;
    float oldRelativeHumidity = m_relativeHumidity;
    float oldCabinTemp = m_cabinTemp;
    float oldPortableWater = m_portableWater;
    float oldGreywaterTank = m_greywaterTank;
    float oldWpaEfficiency = m_wpaEfficiency;
    float oldLiOHScrubbersPercent = m_liOHScrubbersPercent;
    float oldCoolantLoopA = m_coolantLoopA;
    bool oldLiOHScrubbersActive = m_liOHScrubbersActive;

    float totalRespRates = m_astronaut1Resp + m_astronaut2Resp + m_astronaut3Resp + m_astronaut4Resp;
    m_o2PartialPress -= (totalRespRates * 0.0001) * deltaTime;
    m_o2PartialPress = std::clamp(m_o2PartialPress, 0.0f, 100.0f);

    if (!m_liOHScrubbersActive) {
        m_co2Concentration += (totalRespRates * 0.0001) * deltaTime;
    }
    m_co2Concentration = std::clamp(m_co2Concentration, 0.0f, 100.0f);
    
    if (m_cabinPressure != oldCabinPressure) emit cabinPressureChanged();
    if (m_o2PartialPress != oldO2PartialPress) emit o2PartialPressChanged();
    if (m_co2Concentration != oldCO2Concentration) emit co2ConcentrationChanged();
    if (m_relativeHumidity != oldRelativeHumidity) emit relativeHumidityChanged();
    if (m_cabinTemp != oldCabinTemp) emit cabinTempChanged();
    if (m_portableWater != oldPortableWater) emit portableWaterChanged();
    if (m_greywaterTank != oldGreywaterTank) emit greywaterTankChanged();
    if (m_wpaEfficiency != oldWpaEfficiency) emit wpaEfficiencyChanged();
    if (m_liOHScrubbersPercent != oldLiOHScrubbersPercent) emit liOHScrubbersPercentChanged();
    if (m_coolantLoopA != oldCoolantLoopA) emit coolantLoopAChanged();
    if (m_liOHScrubbersActive != oldLiOHScrubbersActive) emit liOHScrubbersActiveChanged();
}