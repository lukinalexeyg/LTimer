#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "precisiontest.h"

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
    PrecisionTest *m_precisionTest;
    int m_stepsCount;
    int m_currentStep;

private:
    void run();
    void stop();
    void changeStep(const int ticksInterval, const double averageFault);
    void setWidgetsEnabled(const bool enabled);
    void save();
};

#endif // MAINWINDOW_H
