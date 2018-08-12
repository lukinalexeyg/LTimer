#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("TickTimer test");
    QApplication::setOrganizationName("Alexey Lukin");
    MainWindow w;
    w.show();

    return a.exec();
}
