#ifndef MISSIONSSYSTEM_H
#define MISSIONSSYSTEM_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <qqmlintegration.h>

class MissionsSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float powerConsumption READ powerConsumption NOTIFY powerConsumptionChanged)
    Q_PROPERTY(QString currentMission READ currentMission NOTIFY currentMissionChanged)
    Q_PROPERTY(QString currentObjective READ currentObjective NOTIFY currentObjectiveChanged)
    Q_PROPERTY(float objectiveProgress READ objectiveProgress NOTIFY objectiveProgressChanged)
    Q_PROPERTY(QString missionStatus READ missionStatus NOTIFY missionStatusChanged)
    Q_PROPERTY(QStringList availableMissions READ availableMissions CONSTANT)

public:
    explicit MissionsSystem(QObject *parent = nullptr);

    float powerConsumption() const;
    QString currentMission() const;
    QString currentObjective() const;
    float objectiveProgress() const;
    QString missionStatus() const;
    QStringList availableMissions() const;

    Q_INVOKABLE void updateSimulation(float deltaTime, float navDistance, float propellant, float velocity);
    Q_INVOKABLE void selectMission(const QString &missionName);
    Q_INVOKABLE void resetMission();

public slots:
    void setPowerConsumption(float power);

signals:
    void powerConsumptionChanged();
    void currentMissionChanged();
    void currentObjectiveChanged();
    void objectiveProgressChanged();
    void missionStatusChanged();

private:
    float m_powerConsumption = 4.0f;
    QString m_currentMission = "Earth Departure";
    QString m_currentObjective = "Execute Trans-Mars Injection (TMI) burn and clear Earth gravity well.";
    float m_objectiveProgress = 0.0f;
    QString m_missionStatus = "IN PROGRESS";
    QStringList m_availableMissions;
};

#endif // MISSIONSSYSTEM_H