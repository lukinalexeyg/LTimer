#include "ltimer.h"

#include <QtMath>



LTimer::LTimer(QObject *parent) :
    QObject(parent),
    m_elapsedTimer(new QElapsedTimer)
{
}



LTimer::~LTimer()
{
    delete m_elapsedTimer;
}



void LTimer::setDuraton(const int duration)
{
    if (m_state == LTimer::Inactive)
        m_duration = duration;
}



void LTimer::setTicksInterval(const int interval)
{
    if (m_state == LTimer::Inactive)
        m_ticksInterval = interval;
}



void LTimer::setTicksCount(const int count)
{
    if (m_state == LTimer::Inactive)
        m_ticksCount= count;
}



void LTimer::setType(const LTimer::Type type)
{
    if (m_state == LTimer::Inactive)
        m_timerType = type;
}



void LTimer::stopWhenTicksOver(const bool stop)
{
    if (m_state == LTimer::Inactive)
        m_stopWhenTicksOver = stop;
}



void LTimer::start()
{
    m_elapsed = 0;
    m_lastTickElapsed = 0;
    m_lastTick = 0;

    m_elapsedTimer->start();

    if (m_ticksInterval >= 0 && m_ticksCount != 0) {
        if (m_tickTimer == nullptr)
            m_tickTimer = newTimer(&LTimer::_tick);
        m_tickTimer->start(m_ticksInterval);
    }
    else if (m_tickTimer != nullptr) {
        m_tickTimer->stop();
        m_tickTimer->deleteLater();
        m_tickTimer = nullptr;
    }

    if (!m_stopWhenTicksOver && m_duration >= 0) {
        if (m_mainTimer == nullptr)
            m_mainTimer = newTimer([this]() {
                stop();
                emit timeout();
            });
        m_mainTimer->start(m_duration);
    }
    else if (m_mainTimer != nullptr) {
        m_mainTimer->stop();
        m_mainTimer->deleteLater();
        m_mainTimer = nullptr;
    }

    m_state = Running;
    emit stateChanged(m_state);
}



template<typename Func>
QTimer *LTimer::newTimer(Func slot)
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



void LTimer::pause()
{
    if (m_state != Running)
        return;

    m_elapsed += m_elapsedTimer->elapsed();

    if (m_tickTimer != nullptr)
        m_tickTimer->stop();

    if (m_mainTimer != nullptr)
        m_mainTimer->stop();

    m_state = Paused;
    emit stateChanged(m_state);
}



void LTimer::resume()
{
    if (m_state != Paused)
        return;

    m_elapsedTimer->restart();

    if (m_tickTimer != nullptr) {
        int timeToTick = m_lastTickElapsed + m_tickTimer->interval() - m_elapsed;
        if (timeToTick < 0)
            timeToTick = 0;

        QTimer::singleShot(timeToTick, this, [this]() {
            if (m_state == Running) {
                _tick();
                m_tickTimer->start(newTickInterval());
            }
        });
    }

    if (m_mainTimer != nullptr)
        m_mainTimer->start(m_duration - m_elapsed);

    m_state = Running;
    emit stateChanged(m_state);
}



void LTimer::stop()
{
    if (m_state == Inactive)
        return;

    m_elapsed += m_elapsedTimer->elapsed();

    if (m_tickTimer != nullptr) {
        m_tickTimer->stop();
        m_tickTimer->deleteLater();
        m_tickTimer = nullptr;
    }

    if (m_mainTimer != nullptr) {
        m_mainTimer->stop();
        m_mainTimer->deleteLater();
        m_mainTimer = nullptr;
    }

    m_state = Inactive;
    emit stateChanged(m_state);
}



int LTimer::elapsed()
{
    if (m_state == Running)
        return static_cast<int>(m_elapsedTimer->elapsed()) + m_elapsed;
    return m_elapsed;
}



int LTimer::remaining()
{
    const int d = _duration();

    if (d < 0)
        return -1;

    int remainingTime = d - m_elapsed;

    if (m_state == Running)
        remainingTime -= m_elapsedTimer->elapsed();

    return remainingTime > 0 ? remainingTime : 0;
}



int LTimer::lastTickRemaining() const
{
    const int d = _duration();

    if (d < 0)
        return -1;

    const int remainingTime = d - m_lastTickElapsed;

    return remainingTime > 0 ? remainingTime : 0;
}



int LTimer::_duration() const
{
    if (!m_stopWhenTicksOver)
        return m_duration;

    return m_ticksInterval*m_ticksCount;
}



void LTimer::_tick()
{
    m_lastTickElapsed += m_ticksInterval;
    m_lastTick++;
    emit tick(m_lastTick);

    if (m_stopWhenTicksOver && m_ticksCount > 0 && m_lastTick >= m_ticksCount) {
        stop();
        emit timeout();
    }
    else
        m_tickTimer->start(newTickInterval());
}



int LTimer::newTickInterval()
{
    if (m_timerType != CoarseStabilized)
        return m_ticksInterval;

    const double newInterval = static_cast<double>(m_lastTickElapsed)
            *static_cast<double>(m_tickTimer->interval())/static_cast<double>(elapsed());

    return qRound(newInterval);
}
