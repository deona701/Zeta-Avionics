#include "LifeSupportSystem.h"
#include <QHostAddress>
#include <QTcpServer>
#include <QFile>

LifeSupportSystem::LifeSupportSystem(QObject *parent) 
: QObject(parent)
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

    m_server->route("/astronaut/1/increaseHeartRate", [this]() { m_astronaut1BPM += 2; emit astronaut1BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseHeartRate", [this]() { m_astronaut1BPM -= 2; emit astronaut1BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/increaseSPO2",      [this]() { m_astronaut1SPO2 += 2; emit astronaut1SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseSPO2",      [this]() { m_astronaut1SPO2 -= 2; emit astronaut1SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/increaseRespRate",  [this]() { m_astronaut1Resp += 2; emit astronaut1RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseRespRate",  [this]() { m_astronaut1Resp -= 2; emit astronaut1RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/increaseTemp",      [this]() { m_astronaut1Temp += 2.0f; emit astronaut1TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/1/decreaseTemp",      [this]() { m_astronaut1Temp -= 2.0f; emit astronaut1TempChanged(); return QHttpServerResponse("text/plain", "OK"); });

    m_server->route("/astronaut/2/increaseHeartRate", [this]() { m_astronaut2BPM += 2; emit astronaut2BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseHeartRate", [this]() { m_astronaut2BPM -= 2; emit astronaut2BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/increaseSPO2",      [this]() { m_astronaut2SPO2 += 2; emit astronaut2SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseSPO2",      [this]() { m_astronaut2SPO2 -= 2; emit astronaut2SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/increaseRespRate",  [this]() { m_astronaut2Resp += 2; emit astronaut2RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseRespRate",  [this]() { m_astronaut2Resp -= 2; emit astronaut2RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/increaseTemp",      [this]() { m_astronaut2Temp += 2.0f; emit astronaut2TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/2/decreaseTemp",      [this]() { m_astronaut2Temp -= 2.0f; emit astronaut2TempChanged(); return QHttpServerResponse("text/plain", "OK"); });

    m_server->route("/astronaut/3/increaseHeartRate", [this]() { m_astronaut3BPM += 2; emit astronaut3BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseHeartRate", [this]() { m_astronaut3BPM -= 2; emit astronaut3BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/increaseSPO2",      [this]() { m_astronaut3SPO2 += 2; emit astronaut3SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseSPO2",      [this]() { m_astronaut3SPO2 -= 2; emit astronaut3SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/increaseRespRate",  [this]() { m_astronaut3Resp += 2; emit astronaut3RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseRespRate",  [this]() { m_astronaut3Resp -= 2; emit astronaut3RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/increaseTemp",      [this]() { m_astronaut3Temp += 2.0f; emit astronaut3TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/3/decreaseTemp",      [this]() { m_astronaut3Temp -= 2.0f; emit astronaut3TempChanged(); return QHttpServerResponse("text/plain", "OK"); });

    m_server->route("/astronaut/4/increaseHeartRate", [this]() { m_astronaut4BPM += 2; emit astronaut4BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseHeartRate", [this]() { m_astronaut4BPM -= 2; emit astronaut4BPMChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/increaseSPO2",      [this]() { m_astronaut4SPO2 += 2; emit astronaut4SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseSPO2",      [this]() { m_astronaut4SPO2 -= 2; emit astronaut4SPO2Changed(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/increaseRespRate",  [this]() { m_astronaut4Resp += 2; emit astronaut4RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseRespRate",  [this]() { m_astronaut4Resp -= 2; emit astronaut4RespChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/increaseTemp",      [this]() { m_astronaut4Temp += 2.0f; emit astronaut4TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
    m_server->route("/astronaut/4/decreaseTemp",      [this]() { m_astronaut4Temp -= 2.0f; emit astronaut4TempChanged(); return QHttpServerResponse("text/plain", "OK"); });
}

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
    
}