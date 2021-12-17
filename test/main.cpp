#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("LTimer test");
    MainWindow w;
    w.show();

    return a.exec();
}
