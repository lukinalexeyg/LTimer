#include <QCoreApplication>
#include <QDebug>
#include <math.h>
#include "../LTimer/ltimer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    LTimer tickTimer;

    qDebug() << "start";
    tickTimer.start();

    for (qint64 i = 0; i < 100000000; i++)
        double value = exp(static_cast<double>(i));

    tickTimer.pause();
    qDebug() << "pause" << tickTimer.elapsed();

    for (qint64 i = 0; i < 1000000000; i++)
        double value = log(static_cast<double>(i));
    tickTimer.resume();
    qDebug() << "resume" << tickTimer.elapsed();

    for (qint64 i = 0; i < 1000000000; i++)
        double value = sqrt(static_cast<double>(i));
    qDebug() << "stop" << tickTimer.elapsed();
    tickTimer.stop();

    return a.exec();
}
