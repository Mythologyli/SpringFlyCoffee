#include <QPushButton>
#include <QTimer>
#include <QDebug>

#include "pumpwindow.h"
#include "ui_pumpwindow.h"

PumpWindow::PumpWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PumpWindow)
{
    ui->setupUi(this);

    pump = new Pump("/dev/ttyUSB0", this);

    connect(pump, &Pump::started, [&]()
    {
        ui->button->setEnabled(false);
        ui->label->setText("正在运行");

        qInfo() << "Pump started";
    });

    connect(pump, &Pump::paused, [&]()
    {
        ui->label->setText("运行暂停！请将杯子靠近接水处");

        qInfo() << "Pump paused";
    });

    connect(pump, &Pump::stopped, [&]()
    {
        ui->label->setText("运行结束！");

        qInfo() << "Pump stopped";

        QTimer::singleShot(1000, [&]()
        {
            ui->button->setEnabled(true);
            emit pumpFinished();
        });
    });

    connect(pump, &Pump::stopped, this, &QWidget::close);
}

PumpWindow::~PumpWindow()
{
    delete ui;
}

void PumpWindow::startPump(int pumpNumber)
{
    ui->button->setEnabled(true);
    ui->label->setText("点击下方按钮开始");

    if (pumpNumber == 1)
    {
        connect(ui->button, &QPushButton::clicked, pump, &Pump::startPump1);

    }
    else if (pumpNumber == 2)
    {
        connect(ui->button, &QPushButton::clicked, pump, &Pump::startPump2);
    }
}
