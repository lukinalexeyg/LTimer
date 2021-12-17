#include "ltimer.h"

#include <QCoreApplication>
#include <QDebug>

#include <math.h>

static constexpr qint64 s_count = 100000000;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    LTimer timer;

    qDebug() << "start";
    timer.start();

    for (qint64 i = 0; i < s_count; ++i)
        exp(i);

    timer.pause();
    qDebug() << "pause" << timer.elapsed();

    for (qint64 i = 0; i < s_count; ++i)
        log(i);

    timer.resume();
    qDebug() << "resume" << timer.elapsed();

    for (qint64 i = 0; i < s_count; ++i)
        sqrt(i);

    qDebug() << "stop" << timer.elapsed();
    timer.stop();
}
