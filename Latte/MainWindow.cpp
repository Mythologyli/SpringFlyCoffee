#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCursor(Qt::BlankCursor);

    setStyleSheet("background-color: black;");

    mode = Mode::Check;

    camera = new Camera("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0");
    faceRecognition = new FaceRecognition(parent);
    timer = new QTimer(this);

    connect(faceRecognition, &FaceRecognition::saveFaceSucceed, this, &QWidget::close);
    connect(faceRecognition, &FaceRecognition::saveFaceSucceed, timer, &QTimer::stop);
    connect(faceRecognition, &FaceRecognition::checkFaceMatch, this, &QWidget::close);
    connect(faceRecognition, &FaceRecognition::checkFaceMatch, timer, &QTimer::stop);

    connect(timer, &QTimer::timeout,
            [&]()
            {
                cv::Mat frame;
                cv::Mat convertedFrame;
                camera->getFrame(frame);
                cv::cvtColor(frame, convertedFrame, CV_BGR2RGB);

                QImage image((uchar *) convertedFrame.data,
                             convertedFrame.cols,
                             convertedFrame.rows,
                             (int) convertedFrame.step,
                             QImage::Format_RGB888);

                ui->imageLabel->setPixmap(QPixmap::fromImage(image));

                rockx_image_t inputImage;
                RockxFace::cvMatToRockxImage(frame, inputImage);

                if (mode == Mode::Check)
                {
                    QString name;

                    faceRecognition->checkFace(inputImage, name);
                    qInfo() << "Check result:" << name;
                }
                else
                {
                    QDateTime date = QDateTime::currentDateTime();
                    QString timeStr = date.toString("yyyyddMMhhmmss");

                    faceRecognition->saveFace(inputImage, timeStr);
                }
            });
    timer->start(80);
}

MainWindow::~MainWindow()
{
    delete camera;
    delete faceRecognition;
    delete timer;

    delete ui;
}

void MainWindow::setMode(Mode recognize_mode)
{
    mode = recognize_mode;
}
