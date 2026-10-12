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

public:
    explicit NavigationSystem(QObject *parent = nullptr);

    float distance() const;
    float velocity() const;
    int eta() const;
    QString currentPos() const;
    QString destination() const;

    Q_INVOKABLE void updateSimulation(float deltaTime, float currentThrust, float currentFuel);

public slots:
    void setDistance(float newDistance);
    void setVelocity(float newVelocity);
    void setEta(float newEta);
    void setCurrentPos(const QString &newPos);
    void setDestination(const QString &newDestination);

signals:
    void distanceChanged();
    void velocityChanged();
    void etaChanged();
    void currentPosChanged();
    void destinationChanged();

private:
    float m_distance = 225000000.0f;
    float m_velocity = 12.0f;
    int m_eta = 217;
    QString m_currentPos = "Earth";
    QString m_destination = "Mars";
};

#endif // NAVIGATIONSYSTEM_H