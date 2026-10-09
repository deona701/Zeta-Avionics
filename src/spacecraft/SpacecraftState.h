#ifndef SPACECRAFTSTATE_H
#define SPACECRAFTSTATE_H

#include <QObject>
#include <qqmlintegration.h>
#include "../systems/PropulsionSystem.h"

class SpacecraftState : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(PropulsionSystem* propulsion READ propulsion CONSTANT)

public:
    explicit SpacecraftState(QObject *parent = nullptr);

    PropulsionSystem* propulsion() { return &m_propulsion; }

    void updateAllSystems(float deltaTime);

private:
    PropulsionSystem m_propulsion;
};

#endif // SPACECRAFTSTATE_H