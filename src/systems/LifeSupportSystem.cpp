#include "LifeSupportSystem.h"
#include <QHostAddress>
#include <QTcpServer>

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
        return "Astronaut #1 - Active";
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

void LifeSupportSystem::updateSimulation(float deltaTime) {
    
}