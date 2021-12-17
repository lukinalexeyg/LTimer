#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ltimer.h"

#include <QMainWindow>

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
    Ui::MainWindow *ui;
    LTimer *m_lTimer;

private:
    void onGet();
    void start();
    void pause();
    void stop();
    void tick(const int tick);
    void setWidgetsEnabled(const bool enabled);
    void setProgressBarValue();
};

#endif // MAINWINDOW_H
