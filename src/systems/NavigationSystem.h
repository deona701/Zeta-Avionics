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
    Q_PROPERTY(QString currentPos READ currentPos WRITE setCurrentPos NOTIFY currentPosChanged)
    Q_PROPERTY(QString destination READ destination WRITE setDestinationDestination NOTIFY destinationChanged)

public:
    explicit NavigationSystem(QObject *parent = nullptr);

    float distance() const;
    QString currentPos() const;
    QString destination() const;

    Q_INVOKABLE void updateSimulation(float deltaTime);

public slots:
    void setDistance(float newDistance);
    void setCurrentPos(const QString &newPos);
    void setDestination(const QString &newDestination);

signals:
    void distanceChanged();
    void currentPosChanged();
    void destinationChanged();

private:
    float m_distance = 225000000.0f;
    QString m_currentPos = "Earth";
    QString m_destination = "Mars";
};

#endif // NAVIGATIONSYSTEM_H