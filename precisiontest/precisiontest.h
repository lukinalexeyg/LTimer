#ifndef PRECISIONTEST_H
#define PRECISIONTEST_H

#include <QObject>
#include "../LTimer/ltimer.h"
#include "ui_mainwindow.h"

const int DEFAULT_MIN_TICK_INTERVAL = 1;
const int DEFAULT_MAX_TICK_INTERVAL = 1000;
const int DEFAULT_TICK_INTERVAL_STEP = 1;
const int DEFAULT_TICKS_COUNT = 100;

class PrecisionTest : public QObject
{
    Q_OBJECT

public:
    explicit PrecisionTest(Ui::MainWindow *ui, QObject *parent = nullptr);
    void run();
    void stop();

signals:
    void step(int ticksInterval, double m_averageFault);
    void done();

private:
    LTimer *m_lTimer;
    Ui::MainWindow *ui;
    double m_faultSums;
    double m_averageFault;

private:
    void tick();
    void timeout();
    void _run(int ticksInterval);
};

#endif // PRECISIONTEST_H
