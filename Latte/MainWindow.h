#ifndef MOCCA_MAIN_WINDOW_H
#define MOCCA_MAIN_WINDOW_H

#include <QWidget>

#include "Camera/Camera.h"
#include "FaceRecognition.h"

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

private:
    Ui::MainWindow *ui;

    Camera *camera;
    FaceRecognition *faceRecognition;
    QTimer *timer;

    Mode mode;
};

#endif //MOCCA_MAIN_WINDOW_H
