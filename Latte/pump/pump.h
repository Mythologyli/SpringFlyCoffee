#ifndef LATTE_PUMP_H
#define LATTE_PUMP_H

#include <QObject>
#include <QSerialPort>

class Pump : public QObject
{
Q_OBJECT
public:
    explicit Pump(const QString &portName, QObject *parent = nullptr);

public slots:

    void startPump1();

    void startPump2();

    void stop();

private slots:

    void onTextMessageReceived(const QString &message);

private:
    QSerialPort *serialPort;

signals:

    void started();

    void stopped();

    void paused();
};

#endif //LATTE_PUMP_H
