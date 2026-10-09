#ifndef SPACECRAFTSIMULATION_H
#define SPACECRAFTSIMULATION_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <qqmlintegration.h>
#include "SpacecraftState.h"

class SpacecraftSimulation : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(SpacecraftState* state READ state CONSTANT)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)

public:
    explicit SpacecraftSimulation(QObject *parent = nullptr);

    SpacecraftState* state() { return &m_state; }
    bool isRunning() const { return m_isRunning; }

    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void setTimeMultiplier(float multiplier);

signals:
    void isRunningChanged();

private slots:
    void tick();

private:
    QTimer m_loopTimer;
    QElapsedTimer m_frameTimer;
    SpacecraftState m_state;

    bool m_isRunning = false;
    float m_timeMultiplier = 1.0f;
};

#endif // SPACECRAFTSIMULATION_H
