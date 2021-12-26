#include "precisiontest.h"

#include <QDebug>



PrecisionTest::PrecisionTest(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    m_lTimer(new LTimer(this)),
    ui(ui)
{
    m_lTimer->stopWhenTicksOver(true);

    connect(m_lTimer, &LTimer::tick, this, &PrecisionTest::tick);
    connect(m_lTimer, &LTimer::timeout, this, &PrecisionTest::timeout);
}



void PrecisionTest::tick()
{
    m_faultSums += static_cast<double>(m_lTimer->lastTickElapsed()) / static_cast<double>(m_lTimer->elapsed());
}



void PrecisionTest::timeout()
{
    m_averageFault = static_cast<double>(m_faultSums) / static_cast<double>(m_lTimer->lastTick());
    emit step(m_lTimer->ticksInterval(), m_averageFault);

    if (m_lTimer->ticksInterval() <= ui->spinBox_maxTicksInterval->value()) {
        const int ticksInterval = m_lTimer->ticksInterval() + ui->spinBox_ticksIntervalStep->value();
        _run(ticksInterval);
    }
    else
        emit done();
}



void PrecisionTest::run()
{
    m_lTimer->setTicksCount(ui->spinBox_ticksCount->value());
    m_lTimer->setTimerType(static_cast<LTimer::Type>(ui->comboBox_type->currentIndex()));
    _run(ui->spinBox_minTicksInterval->value());
}



void PrecisionTest::_run(int ticksInterval)
{
    m_faultSums = 0;
    m_averageFault = 0;
    m_lTimer->setTicksInterval(ticksInterval);
    m_lTimer->start();
}



void PrecisionTest::stop()
{
    m_lTimer->stop();
}
