#include "LifeSupportSystem.h"
#include <QHostAddress>
#include <QTcpServer>
#include <QFile>

LifeSupportSystem::LifeSupportSystem(QObject *parent) 
: QObject(parent)
, m_astronaut1Temp(36.8f)
, m_astronaut1BPM(72)
, m_astronaut1SPO2(98)
, m_astronaut1Resp(12)
, m_astronaut2Temp(36.8f)
, m_astronaut2BPM(72)
, m_astronaut2SPO2(98)
, m_astronaut2Resp(12)
, m_astronaut3Temp(36.8f)
, m_astronaut3BPM(72)
, m_astronaut3SPO2(98)
, m_astronaut3Resp(12)
, m_astronaut4Temp(36.8f)
, m_astronaut4BPM(72)
, m_astronaut4SPO2(98)
, m_astronaut4Resp(12)
, m_server(new QHttpServer(this))
{
    auto tcpServer = new QTcpServer(this);
    if (tcpServer->listen(QHostAddress::Any, 8080)) {
        m_server->bind(tcpServer);
    }

    m_server->route("/astronaut1", []() {
        QFile file(":/qt/qml/Zeta_Avionics/assets/web/astronaut_control.html");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray content = file.readAll();
            return QHttpServerResponse("text/html", content);
        }
        return QHttpServerResponse("text/html", QByteArray("Error loading control panel."));
    });
    m_server->route("/astronaut2", []() {
        return "Astronaut #2 - Active";
    });
    m_server->route("/astronaut3", []() {
        return "Astronaut #3 - Active";
    });
    m_server->route("/astronaut4", []() {
        return "Astronaut #4 - Active";
    });
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