#ifndef LATTE_MAINWINDOW_H
#define LATTE_MAINWINDOW_H

#include <QWidget>

#include "Camera/Camera.h"
#include "FaceRecognition/FaceRecognition.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    enum Mode
    {
        Check,
        Save
    };

    void setMode(Mode recognize_mode);

private slots:

    void drawRectangle(int left, int bottom, int right, int top);

private:
    Ui::MainWindow *ui;

    Camera *camera;
    FaceRecognition *faceRecognition;
    QTimer *timer;

    Mode mode;

    int boxLeft, boxBottom, boxRight, boxTop;
};

#endif //LATTE_MAINWINDOW_H
