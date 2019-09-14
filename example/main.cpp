#include <QCoreApplication>
#include <QDebug>
#include <math.h>
#include "../LTimer/ltimer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    LTimer lTimer;

    qDebug() << "start";
    lTimer.start();

    for (qint64 i = 0; i < 100000000; i++)
        double value = exp(static_cast<double>(i));

    lTimer.pause();
    qDebug() << "pause" << lTimer.elapsed();

    for (qint64 i = 0; i < 1000000000; i++)
        double value = log(static_cast<double>(i));
    lTimer.resume();
    qDebug() << "resume" << lTimer.elapsed();

    for (qint64 i = 0; i < 1000000000; i++)
        double value = sqrt(static_cast<double>(i));
    qDebug() << "stop" << lTimer.elapsed();
    lTimer.stop();

    return a.exec();
}
