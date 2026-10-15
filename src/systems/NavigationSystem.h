#ifndef NAVIGATIONSYSTEM_H
#define NAVIGATIONSYSTEM_H

#include <QObject>
#include <QString>
#include <qqmlintegration.h>

class NavigationSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float distance READ distance WRITE setDistance NOTIFY distanceChanged)
    Q_PROPERTY(float velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)
    Q_PROPERTY(int eta READ eta WRITE setEta NOTIFY etaChanged)
    Q_PROPERTY(QString currentPos READ currentPos WRITE setCurrentPos NOTIFY currentPosChanged)
    Q_PROPERTY(QString destination READ destination WRITE setDestination NOTIFY destinationChanged)
    Q_PROPERTY(QString trajectory READ trajectory WRITE setTrajectory NOTIFY trajectoryChanged)

public:
    explicit NavigationSystem(QObject *parent = nullptr);

    float distance() const;
    float velocity() const;
    int eta() const;
    QString currentPos() const;
    QString destination() const;
    QString trajectory() const;

    Q_INVOKABLE void updateSimulation(float deltaTime, float currentThrust, float currentFuel, bool engineStatus);

public slots:
    void setDistance(float newDistance);
    void setVelocity(float newVelocity);
    void setEta(int newEta);
    void setCurrentPos(const QString &newPos);
    void setDestination(const QString &newDestination);
    void setTrajectory(const QString &newTrajectory);

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