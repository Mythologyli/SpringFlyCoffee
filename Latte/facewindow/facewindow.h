#ifndef LATTE_FACEWINDOW_H
#define LATTE_FACEWINDOW_H

#include <QWidget>

#include "../pumpwindow/pumpwindow.h"
#include "../camera/camera.h"
#include "../facerecognition/facerecognition.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class FaceWindow;
}
QT_END_NAMESPACE

class FaceWindow : public QWidget
{
Q_OBJECT

public:
    explicit FaceWindow(QWidget *parent = nullptr);

    ~FaceWindow() override;

    enum Mode
    {
        Check,
        Save
    };

    void setMode(Mode recognize_mode);

private slots:

    void setFaceBox(rockx_rect_t box);

private:
    Ui::FaceWindow *ui;

    PumpWindow *pumpWindow;

    Camera *camera;
    FaceRecognition *faceRecognition;

    QTimer *cameraTimer;
    QTimer *faceRecognitionTimer;

    cv::Mat cameraFrame;

    Mode mode;

    rockx_rect_t faceBox;
};

#endif //LATTE_FACEWINDOW_H
