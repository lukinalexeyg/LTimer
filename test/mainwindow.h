#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../src/ticktimer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const QStringList TYPES = {"Precise", "Coarse", "VeryCoarse", "CoarseStabilized"};
    const QStringList STATES = {"Inactive", "Running", "Paused"};
    const int DEFAULT_DURATION = 20000;
    const int DEFAULT_TICKS_INTERVAL = 200;
    const int DEFAULT_TICKS_COUNT = 100;

    Ui::MainWindow *ui;
    TickTimer *timer;

    void setWidgetsEnabled(bool enabled);
    void setProgressBarValue();
};

#endif // MAINWINDOW_H
