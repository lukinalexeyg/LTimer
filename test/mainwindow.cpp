#include <QSpinBox>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new TickTimer(this))
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    ui->spinBox_duration->setValue(DEFAULT_DURATION);
    ui->spinBox_ticksInterval->setValue(DEFAULT_TICKS_INTERVAL);
    ui->spinBox_ticksCount->setValue(DEFAULT_TICKS_COUNT);
    ui->comboBox_type->insertItems(0, TYPES);
    ui->comboBox_type->setCurrentIndex(timer->type());
    ui->checkBox_stopWhenTicksOver->setChecked(timer->willStopWhenTicksOver());

    timer->setDuraton(DEFAULT_DURATION);
    timer->setTicksInterval(DEFAULT_TICKS_INTERVAL);
    timer->setTicksCount(DEFAULT_TICKS_COUNT);

    setProgressBarValue();

    connect(ui->pushButton_get, &QPushButton::released, this, [this]() {
        ui->lineEdit_elapsed->setText(QString::number(timer->elapsed()));
        ui->lineEdit_remaining->setText(QString::number(timer->remaining()));
        ui->lineEdit_lastTickElapsed->setText(QString::number(timer->lastTickElapsed()));
        ui->lineEdit_lastTickRemaining->setText(QString::number(timer->lastTickRemaining()));
    });

    connect(ui->pushButton_start, &QPushButton::released, this, [this]() {
        setWidgetsEnabled(false);
        timer->setDuraton(ui->spinBox_duration->value());
        timer->setTicksInterval(ui->spinBox_ticksInterval->value());
        timer->setTicksCount(ui->spinBox_ticksCount->value());
        timer->setType(static_cast<TickTimer::Type>(ui->comboBox_type->currentIndex()));
        timer->stopWhenTicksOver(ui->checkBox_stopWhenTicksOver->isChecked());
        timer->start();
        setProgressBarValue();
        qDebug() << "start ===========================================================================";
    });

    connect(ui->pushButton_pause, &QPushButton::released, this, [this]() {
        if (timer->state() == TickTimer::Running) {
            timer->pause();
            qDebug() << "pause ===========================================================================";
            qDebug() << " elapsed =" << timer->elapsed()
                     << " remaining =" << timer->remaining()
                     << " lastTickElapsed =" << timer->lastTickElapsed()
                     << " lastTickRemaining =" << timer->lastTickRemaining();
        }
    });

    connect(ui->pushButton_resume, &QPushButton::released, timer, &TickTimer::resume);

    connect(ui->pushButton_stop, &QPushButton::released, this, [this]() {
        setWidgetsEnabled(true);
        timer->stop();
        setProgressBarValue();
    });

    connect(timer, &TickTimer::tick, this, [this](int tick) {
        setProgressBarValue();
        qDebug() << "tick =" << tick
                 << " elapsed =" << timer->elapsed()
                 << " remaining =" << timer->remaining()
                 << " lastTickElapsed =" << timer->lastTickElapsed()
                 << " lastTickRemaining =" << timer->lastTickRemaining();
    });

    connect(timer, &TickTimer::stateChanged, this, [this](int state) {ui->statusBar->showMessage("Current state: "+STATES.at(state));});
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::setWidgetsEnabled(bool enabled)
{
    ui->checkBox_stopWhenTicksOver->setEnabled(enabled);
    ui->spinBox_duration->setEnabled(enabled);
    ui->spinBox_ticksInterval->setEnabled(enabled);
    ui->spinBox_ticksCount->setEnabled(enabled);
    ui->comboBox_type->setEnabled(enabled);
}



void MainWindow::setProgressBarValue()
{
    ui->progressBar->setValue(timer->lastTick());

    if (timer->ticksCount() > 0) {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(timer->ticksCount());
    }
    else {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(INT_MAX);
    }

    QString text = QString("%1 / %2").arg(timer->lastTick()).arg(timer->ticksCount());
    ui->progressBar->setFormat(text);
}
