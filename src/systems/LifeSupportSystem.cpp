#include "LifeSupportSystem.h"
#include <QHostAddress>
#include <QTcpServer>

LifeSupportSystem::LifeSupportSystem(QObject *parent) 
: QObject(parent)
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