#include <QCoreApplication>
#include <QDebug>

#include "pump.h"

Pump::Pump(const QString &portName, QObject *parent)
{
    serialPort = new QSerialPort(portName, this);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->open(QIODevice::ReadWrite);

    connect(serialPort, &QSerialPort::readyRead,
            this, [&]()
            {
                QByteArray data = serialPort->readAll();
                emit onTextMessageReceived(data);
            });

    connect(serialPort, &QSerialPort::errorOccurred, [&]()
    {
        qWarning() << "Pump serial error: " << serialPort->errorString();
    });
}

void Pump::startPump1()
{
    serialPort->write("1");
    serialPort->flush();
    qInfo() << "Send pump1 start command.";
}

void Pump::startPump2()
{
    serialPort->write("2");
    serialPort->flush();
    qInfo() << "Send pump2 start command.";
}

void Pump::stop()
{
    serialPort->write("0");
    serialPort->flush();
    qInfo() << "Send pump stop command.";
}

void Pump::onTextMessageReceived(const QString &message)
{
    qInfo() << "Pump serial receive:" << message;

    if (message.contains('0'))
    {
        emit stopped();
    }
    else if (message.contains('1'))
    {
        emit started();
    }
    else if (message.contains('2'))
    {
        emit paused();
    }
}
