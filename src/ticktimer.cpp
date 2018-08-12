#include "ticktimer.h"
#include <QtMath>

TickTimer::TickTimer(QObject *parent) :
    QObject(parent),
    elapsedTimer(new QElapsedTimer)
{}



void TickTimer::stopWhenTicksOver(bool stop)
{
    if (m_state == TickTimer::Inactive)
        m_stopWhenTicksOver = stop;
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
        m_type = type;
}



void TickTimer::start()
{
    m_elapsed = 0;
    m_lastTickElapsed = 0;
    m_lastTick = 0;

    elapsedTimer->start();

    if (m_ticksInterval >= 0 && m_ticksCount != 0) {
        if (tickTimer == Q_NULLPTR) {
            tickTimer = new QTimer(this);
            startTimer(tickTimer, this, &TickTimer::_tick);
        }
        tickTimer->start(m_ticksInterval);
    }
    else if (tickTimer != Q_NULLPTR) {
        tickTimer->stop();
        tickTimer->deleteLater();
        tickTimer = Q_NULLPTR;
    }

    if (!m_stopWhenTicksOver && m_duration >= 0) {
        if (mainTimer == Q_NULLPTR) {
            mainTimer = new QTimer(this);
            startTimer(mainTimer, this, [this]() {
                stop();
                emit timeout();
            });
        }
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



template <class Obj, typename Func1>
void TickTimer::startTimer(QTimer *timer, const Obj *object, Func1 slot)
{
    timer->setSingleShot(true);
    if (m_type != CoarseStabilized)
        timer->setTimerType(static_cast<Qt::TimerType>(m_type));
    else
        timer->setTimerType(Qt::TimerType::CoarseTimer);
    connect(timer, &QTimer::timeout, object, slot);
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
            int timeToTick = m_ticksInterval + m_lastTickElapsed - m_elapsed;
            if (timeToTick < 0)
                timeToTick = 0;

            QTimer::singleShot(timeToTick, this, [this]() {
                if (m_state == Running) {
                    _tick();
                    tickTimer->start(_ticksInterval());
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
        tickTimer->start(_ticksInterval());
}



int TickTimer::_ticksInterval()
{
    if (m_type != CoarseStabilized)
        return m_ticksInterval;

    double newInterval = static_cast<double>(m_lastTickElapsed)*static_cast<double>(tickTimer->interval())/static_cast<double>(elapsed());
    return qRound(newInterval);
}
