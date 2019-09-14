#ifndef LTIMER_H
#define LTIMER_H

#pragma once

#include <QElapsedTimer>
#include <QTimer>

class LTimer : public QObject
{
    Q_OBJECT

public:
    enum State {
        Inactive,
        Running,
        Paused
    };

    enum Type {
        Precise = Qt::TimerType::PreciseTimer,
        Coarse = Qt::TimerType::CoarseTimer,
        VeryCoarse = Qt::TimerType::VeryCoarseTimer,
        CoarseStabilized
    };

    LTimer(QObject *parent = Q_NULLPTR);
    ~LTimer();

    void setDuraton(int duration);
    inline int duration()                       { return m_duration; }

    void setTicksInterval(int interval);
    inline int ticksInterval()                  { return m_ticksInterval; }
    void setTicksCount(int count);
    inline int ticksCount()                     { return m_ticksCount; }

    void stopWhenTicksOver(bool stop);
    inline bool willStopWhenTicksOver()         { return m_stopWhenTicksOver; }

    void setType(LTimer::Type timerType);
    inline LTimer::Type timerType()             { return m_timerType; }
    inline QElapsedTimer::ClockType clockType() { return elapsedTimer->clockType(); }
    inline bool isMonotonic()                   { return elapsedTimer->isMonotonic(); }

    void start();
    void pause();
    void resume();
    void stop();
    inline LTimer::State state()                { return m_state; }

    int elapsed();
    int remaining();
    inline int lastTickElapsed()                { return m_lastTickElapsed; }
    int lastTickRemaining();
    inline int lastTick()                       { return m_lastTick; }

signals:
    void tick(int);
    void timeout();
    void stateChanged(int);

private:
    QElapsedTimer *elapsedTimer;
    QTimer *tickTimer = Q_NULLPTR;
    QTimer *mainTimer = Q_NULLPTR;

    bool m_stopWhenTicksOver = false;
    int m_duration = -1;
    int m_ticksInterval = 1000;
    int m_ticksCount = -1;
    Type m_timerType = CoarseStabilized;
    State m_state = Inactive;

    int m_elapsed = 0;
    int m_lastTickElapsed = 0;
    int m_lastTick = 0;

    template<typename Func> QTimer *newTimer(Func slot);
    int _duration();
    void _tick();
    int newTickInterval();
};

#endif // LTIMER_H
