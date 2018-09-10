#include "ticktimer.h"
#include <QtMath>

TickTimer::TickTimer(QObject *parent) :
    QObject(parent),
    elapsedTimer(new QElapsedTimer)
{}



TickTimer::~TickTimer()
{
    delete elapsedTimer;
}



void TickTimer::setDuraton(int duration)
{
    if (m_state == TickTimer::Inactive)
        m_duration = duration;
}



void TickTimer::setTicksInterval(int interval)
{
    if (m_state == TickTimer::Inactive)
        m_ticksInterval = interval;
}



void TickTimer::setTicksCount(int count)
{
    if (m_state == TickTimer::Inactive)
        m_ticksCount= count;
}



void TickTimer::setType(TickTimer::Type type)
{
    if (m_state == TickTimer::Inactive)
        m_timerType = type;
}



void TickTimer::stopWhenTicksOver(bool stop)
{
    if (m_state == TickTimer::Inactive)
        m_stopWhenTicksOver = stop;
}



void TickTimer::start()
{
    m_elapsed = 0;
    m_lastTickElapsed = 0;
    m_lastTick = 0;

    elapsedTimer->start();

    if (m_ticksInterval >= 0 && m_ticksCount != 0) {
        if (tickTimer == Q_NULLPTR)
            tickTimer = newTimer(&TickTimer::_tick);
        tickTimer->start(m_ticksInterval);
    }
    else if (tickTimer != Q_NULLPTR) {
        tickTimer->stop();
        tickTimer->deleteLater();
        tickTimer = Q_NULLPTR;
    }

    if (!m_stopWhenTicksOver && m_duration >= 0) {
        if (mainTimer == Q_NULLPTR)
            mainTimer = newTimer([this]() {
                stop();
                emit timeout();
            });
        mainTimer->start(m_duration);
    }
    else if (mainTimer != Q_NULLPTR) {
        mainTimer->stop();
        mainTimer->deleteLater();
        mainTimer = Q_NULLPTR;
    }

    m_state = Running;
    emit stateChanged(m_state);
}



template<typename Func>
QTimer *TickTimer::newTimer(Func slot)
{
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);

    if (m_timerType != CoarseStabilized)
        timer->setTimerType(static_cast<Qt::TimerType>(m_timerType));
    else
        timer->setTimerType(Qt::TimerType::CoarseTimer);

    connect(timer, &QTimer::timeout, this, slot);
    return timer;
}



void TickTimer::pause()
{
    if (m_state == Running) {
        m_elapsed += elapsedTimer->elapsed();

        if (tickTimer != Q_NULLPTR)
            tickTimer->stop();

        if (mainTimer != Q_NULLPTR)
            mainTimer->stop();

        m_state = Paused;
        emit stateChanged(m_state);
    }
}



void TickTimer::resume()
{
    if (m_state == Paused) {
        elapsedTimer->restart();

        if (tickTimer != Q_NULLPTR) {
            int timeToTick = m_lastTickElapsed + tickTimer->interval() - m_elapsed;
            if (timeToTick < 0)
                timeToTick = 0;

            QTimer::singleShot(timeToTick, this, [this]() {
                if (m_state == Running) {
                    _tick();
                    tickTimer->start(newTickInterval());
                }
            });
        }

        if (mainTimer != Q_NULLPTR)
            mainTimer->start(m_duration - m_elapsed);

        m_state = Running;
        emit stateChanged(m_state);
    }
}



void TickTimer::stop()
{
    if (m_state != Inactive) {
        m_elapsed += elapsedTimer->elapsed();

        if (tickTimer != Q_NULLPTR) {
            tickTimer->stop();
            tickTimer->deleteLater();
            tickTimer = Q_NULLPTR;
        }

        if (mainTimer != Q_NULLPTR) {
            mainTimer->stop();
            mainTimer->deleteLater();
            mainTimer = Q_NULLPTR;
        }

        m_state = Inactive;
        emit stateChanged(m_state);
    }
}



int TickTimer::elapsed()
{
    if (m_state == Running)
        return static_cast<int>(elapsedTimer->elapsed()) + m_elapsed;
    return m_elapsed;
}



int TickTimer::remaining()
{
    int d = _duration();

    if (d < 0)
        return -1;

    int remainingTime = d - m_elapsed;

    if (m_state == Running)
        remainingTime -= elapsedTimer->elapsed();

    return remainingTime > 0 ? remainingTime : 0;
}



int TickTimer::lastTickRemaining()
{
    int d = _duration();

    if (d < 0)
        return -1;

    int remainingTime = d - m_lastTickElapsed;

    return remainingTime > 0 ? remainingTime : 0;
}



int TickTimer::_duration()
{
    if (!m_stopWhenTicksOver)
        return m_duration;

    return m_ticksInterval*m_ticksCount;
}



void TickTimer::_tick()
{
    m_lastTickElapsed += m_ticksInterval;
    m_lastTick++;
    emit tick(m_lastTick);

    if (m_stopWhenTicksOver && m_ticksCount > 0 && m_lastTick >= m_ticksCount) {
        stop();
        emit timeout();
    }
    else
        tickTimer->start(newTickInterval());
}



int TickTimer::newTickInterval()
{
    if (m_timerType != CoarseStabilized)
        return m_ticksInterval;

    double newInterval = static_cast<double>(m_lastTickElapsed)*static_cast<double>(tickTimer->interval())/static_cast<double>(elapsed());
    return qRound(newInterval);
}
