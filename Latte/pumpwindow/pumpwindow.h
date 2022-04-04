#ifndef LATTE_PUMPWINDOW_H
#define LATTE_PUMPWINDOW_H

#include <QWidget>

#include "../pump/pump.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class PumpWindow;
}
QT_END_NAMESPACE

class PumpWindow : public QWidget
{
Q_OBJECT

public:
    explicit PumpWindow(QWidget *parent = nullptr);

    ~PumpWindow() override;

public slots:

    void startPump(int pumpNumber);

private:
    Ui::PumpWindow *ui;

    Pump *pump;

signals:

    void pumpFinished();

};

#endif //LATTE_PUMPWINDOW_H
