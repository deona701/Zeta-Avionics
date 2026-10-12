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

    static constexpr float GRAVITY = 9.80665f;
    static constexpr float DRY_MASS = 1000.0f;
    static constexpr float MAX_PROPELLANT_MASS = 2000.0f;

private:
    PropulsionSystem m_propulsion;
    NavigationSystem m_navigation;
};

#endif // SPACECRAFTSTATE_H