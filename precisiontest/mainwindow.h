#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "precisiontest.h"

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
    const QStringList TYPE_STRINGS = { "Precise", "Coarse", "VeryCoarse", "CoarseStabilized" };

    Ui::MainWindow *ui;
    PrecisionTest *m_precisionTest;
    int m_stepsCount;
    int m_currentStep;

private:
    void run();
    void stop();
    void changeStep(int ticksInterval, double averageFault);
    void setWidgetsEnabled(bool enabled);
    void save();
};

#endif // MAINWINDOW_H
