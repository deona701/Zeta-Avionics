#ifndef LIFESUPPORTSYSTEM_H
#define LIFESUPPORTSYSTEM_H

#include <QObject>
#include <QString>
#include <qqmlintegration.h>

class LifeSupportSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT


public:
    explicit LifeSupportSystem(QObject *parent = nullptr);


    
    Q_INVOKABLE void updateSimulation(float deltaTime);
};

#endif // LIFESUPPORTSYSTEM_H
