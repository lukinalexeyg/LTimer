#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("LTimer precision test");
    QApplication::setOrganizationName("Alexey Lukin");
    MainWindow w;
    w.show();

    return a.exec();
}
