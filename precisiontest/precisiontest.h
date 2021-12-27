#ifndef PRECISIONTEST_H
#define PRECISIONTEST_H

#include "ltimer.h"
#include "ui_mainwindow.h"

class PrecisionTest : public QObject
{
    Q_OBJECT

public:
    explicit PrecisionTest(Ui::MainWindow *ui, QObject *parent = nullptr);

    void run();
    void stop();

signals:
    void step(const int ticksInterval, const double m_averageFault);
    void done();

private:
    LTimer *m_lTimer;
    Ui::MainWindow *ui;
    double m_faultSums;
    double m_averageFault;

private:
    void onTimerTick(const int tick);
    void onTimerStopped();
    void _run(const int ticksInterval);
};

#endif // PRECISIONTEST_H
