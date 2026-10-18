#ifndef NAVIGATIONSYSTEM_H
#define NAVIGATIONSYSTEM_H

#include <QObject>
#include <QString>
#include <qqmlintegration.h>

class NavigationSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(float velocity READ velocity NOTIFY velocityChanged)
    Q_PROPERTY(int eta READ eta NOTIFY etaChanged)
    Q_PROPERTY(QString currentPos READ currentPos NOTIFY currentPosChanged)
    Q_PROPERTY(QString destination READ destination NOTIFY destinationChanged)
    Q_PROPERTY(QString trajectory READ trajectory NOTIFY trajectoryChanged)

public:
    explicit NavigationSystem(QObject *parent = nullptr);

    float distance() const;
    float velocity() const;
    int eta() const;
    QString currentPos() const;
    QString destination() const;
    QString trajectory() const;

    Q_INVOKABLE void updateSimulation(float deltaTime, float currentThrust, float currentFuel, bool engineStatus);

signals:
    void distanceChanged();
    void velocityChanged();
    void etaChanged();
    void currentPosChanged();
    void destinationChanged();
    void trajectoryChanged();

private:
    float m_distance = 225000000.0f;
    float m_velocity = 12.0f;
    int m_eta = 217;
    QString m_currentPos = "Earth";
    QString m_destination = "Mars";
    QString m_trajectory = "Nominal Coast";
};

#endif // NAVIGATIONSYSTEM_H