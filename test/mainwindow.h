#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../LTimer/ltimer.h"

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
    const QStringList TYPES_STRINGS = { "Precise", "Coarse", "VeryCoarse", "CoarseStabilized" };
    const QStringList STATES_STRINGS = { "Inactive", "Running", "Paused" };
    const int DEFAULT_DURATION = 20000;
    const int DEFAULT_TICKS_INTERVAL = 200;
    const int DEFAULT_TICKS_COUNT = 100;

    Ui::MainWindow *ui;
    LTimer *m_lTimer;

private:
    void onGet();
    void start();
    void pause();
    void stop();
    void tick(int tick);
    void setWidgetsEnabled(bool enabled);
    void setProgressBarValue();
};

#endif // MAINWINDOW_H
