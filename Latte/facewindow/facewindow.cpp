#include <QTimer>
#include <QDebug>
#include <QDateTime>

#include "facewindow.h"
#include "ui_facewindow.h"

FaceWindow::FaceWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FaceWindow),
        faceBox{-10, -10, -10, -10}
{
    ui->setupUi(this);

    setCursor(Qt::BlankCursor);

    mode = Mode::Check;

    pumpWindow = new PumpWindow(this);
    pumpWindow->hide();

    camera = new Camera("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0");
    faceRecognition = new FaceRecognition(this);
    serviceBot = new ServiceBot(this);
    xp58 = new Xp58();

    cameraTimer = new QTimer(this);
    faceRecognitionTimer = new QTimer(this);

    connect(faceRecognition, &FaceRecognition::faceBoxGet, this, &FaceWindow::setFaceBox);
    connect(faceRecognition, &FaceRecognition::faceSaved, this, &QWidget::close);
    connect(faceRecognition, &FaceRecognition::faceSaved, faceRecognitionTimer, &QTimer::stop);

    connect(faceRecognition, &FaceRecognition::faceMatched, [&](const QString& name)
    {
        faceRecognitionTimer->stop();
        ui->imageLabel->clear();

        pumpWindow->showFullScreen();

        if (name == "20220421012808")
        {
            serviceBot->sendMessage("2680196372", "你于 " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " 使用了咖啡机！\n欢迎再次使用！");
            xp58->printLine("你于 " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " 使用了咖啡机！");
            xp58->feedNLines(5);
            xp58->printLine("欢迎再次使用!");
            xp58->feedNLines(10);

            pumpWindow->startPump(1);
        }
        else
        {
            serviceBot->sendMessage("838997750", "你于 " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " 使用了咖啡机！\n欢迎再次使用！");
            xp58->printLine("你于 " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " 使用了咖啡机！");
            xp58->feedNLines(5);
            xp58->printLine("欢迎再次使用!");
            xp58->feedNLines(10);

            pumpWindow->startPump(2);
        }
    });

    connect(pumpWindow, &PumpWindow::pumpFinished, [&]()
    {
        faceRecognitionTimer->start(80);

        pumpWindow->hide();
    });

    connect(cameraTimer, &QTimer::timeout, [&]()
    {
        camera->getFrame(cameraFrame);
    });

    connect(faceRecognitionTimer, &QTimer::timeout,
            [&]()
            {
                cv::Mat convertedFrame;
                cv::rectangle(cameraFrame,
                              cv::Point(faceBox.left, faceBox.bottom),
                              cv::Point(faceBox.right, faceBox.top),
                              cv::Scalar(0, 255, 0),
                              3);

                faceBox.left = -10;
                faceBox.bottom = -10;
                faceBox.right = -10;
                faceBox.top = -10;

                cv::cvtColor(cameraFrame, convertedFrame, CV_BGR2RGB);

                QImage image((uchar *) convertedFrame.data,
                             convertedFrame.cols,
                             convertedFrame.rows,
                             (int) convertedFrame.step,
                             QImage::Format_RGB888);

                ui->imageLabel->setPixmap(QPixmap::fromImage(image));

                rockx_image_t inputImage;
                RockxFace::cvMatToRockxImage(cameraFrame, inputImage);

                if (mode == Mode::Check)
                {
                    QString name;

                    faceRecognition->checkFace(inputImage, name);
                    qInfo() << "Check result:" << name;
                }
                else
                {
                    QDateTime date = QDateTime::currentDateTime();
                    QString timeStr = date.toString("yyyyMMddhhmmss");

                    faceRecognition->saveFace(inputImage, timeStr);
                }
            });

    cameraTimer->start(80);
    faceRecognitionTimer->start(80);
}

FaceWindow::~FaceWindow()
{
    delete camera;
    delete xp58;

    delete ui;
}

void FaceWindow::setMode(Mode recognize_mode)
{
    mode = recognize_mode;
}

void FaceWindow::setFaceBox(rockx_rect_t box)
{
    faceBox.left = box.left;
    faceBox.bottom = box.bottom;
    faceBox.right = box.right;
    faceBox.top = box.top;
}
