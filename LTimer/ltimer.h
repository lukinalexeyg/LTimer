#ifndef LTIMER_H
#define LTIMER_H

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

public:
    LTimer(QObject *parent = nullptr);
    ~LTimer();

    LTimer::State state() const                 { return m_state; }

    void setDuraton(const int duration);
    int duration() const                        { return m_duration; }

    void setTicksInterval(const int interval);
    int ticksInterval() const                   { return m_ticksInterval; }

    void setTicksCount(const int count);
    int ticksCount() const                      { return m_ticksCount; }

    void stopWhenTicksOver(const bool stop);
    bool willStopWhenTicksOver() const          { return m_stopWhenTicksOver; }

    void setType(const LTimer::Type timerType);
    LTimer::Type timerType() const              { return m_timerType; }

    QElapsedTimer::ClockType clockType() const  { return m_elapsedTimer->clockType(); }
    bool isMonotonic() const                    { return m_elapsedTimer->isMonotonic(); }

    void start();
    void pause();
    void resume();
    void stop();    

    int elapsed();
    int remaining();
    int lastTickElapsed() const                 { return m_lastTickElapsed; }
    int lastTickRemaining() const;
    int lastTick() const                        { return m_lastTick; }

signals:
    void tick(int);
    void timeout();
    void stateChanged(int);

private:
    QElapsedTimer *m_elapsedTimer;
    QTimer *m_tickTimer = nullptr;
    QTimer *m_mainTimer = nullptr;

    bool m_stopWhenTicksOver = false;
    int m_duration = -1;
    int m_ticksInterval = 1000;
    int m_ticksCount = -1;
    Type m_timerType = CoarseStabilized;
    State m_state = Inactive;

    int m_elapsed = 0;
    int m_lastTickElapsed = 0;
    int m_lastTick = 0;

private:
    template<typename Func> QTimer *newTimer(Func slot);
    int _duration() const;
    void _tick();
    int newTickInterval();
};

#endif // LTIMER_H
