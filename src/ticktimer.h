#ifndef TICKTIMER_H
#define TICKTIMER_H

#pragma once

#include <QElapsedTimer>
#include <QTimer>

class TickTimer : public QObject
{
    Q_OBJECT

public:
    enum State {
        Inactive = 0,
        Running,
        Paused
    };

    enum Type {
        Precise = Qt::TimerType::PreciseTimer,
        Coarse = Qt::TimerType::CoarseTimer,
        VeryCoarse = Qt::TimerType::VeryCoarseTimer,
        CoarseStabilized
    };

    TickTimer(QObject *parent = nullptr);
    inline ~TickTimer()                                 {delete elapsedTimer;}

    void setDuraton(int duration);
    void setTicksInterval(int interval);
    void setTicksCount(int count);
    void stopWhenTicksOver(bool stop);
    void setType(TickTimer::Type type);

    inline int duration()                               {return m_duration;}
    inline int ticksInterval()                          {return m_ticksInterval;}
    inline int ticksCount()                             {return m_ticksCount;}
    inline bool willStopWhenTicksOver()                 {return m_stopWhenTicksOver;}
    inline TickTimer::Type type()                       {return m_type;}

    void start();
    void pause();
    void resume();
    void stop();
    inline TickTimer::State state()                     {return m_state;}

    int elapsed();
    int remaining();
    inline int lastTickElapsed()                        {return m_lastTickElapsed;}
    int lastTickRemaining();
    inline int lastTick()                               {return m_lastTick;}

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
    Type m_type = CoarseStabilized;
    State m_state = Inactive;

    int m_elapsed = 0;
    int m_lastTickElapsed = 0;
    int m_lastTick = 0;

    template <class Obj, typename Func1>
    void startTimer(QTimer *timer, const Obj *object, Func1 slot);
    int _duration();
    void _tick();
    int _ticksInterval();
};

#endif // TICKTIMER_H