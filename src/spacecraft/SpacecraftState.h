#ifndef SPACECRAFTSTATE_H
#define SPACECRAFTSTATE_H

#include <QObject>
#include <qqmlintegration.h>
#include "../systems/PropulsionSystem.h"
#include "../systems/NavigationSystem.h"

class SpacecraftState : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(PropulsionSystem* propulsion READ propulsion CONSTANT)
    Q_PROPERTY(NavigationSystem* navigation READ navigation CONSTANT)

public:
    explicit SpacecraftState(QObject *parent = nullptr);

    PropulsionSystem* propulsion() { return &m_propulsion; }
    NavigationSystem* navigation() { return &m_navigation; }

    void updateAllSystems(float deltaTime);

private:
    PropulsionSystem m_propulsion;
    NavigationSystem m_navigation;
};

#endif // SPACECRAFTSTATE_H