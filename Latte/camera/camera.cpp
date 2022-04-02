#include <QDebug>

#include "camera.h"

Camera::Camera(const char *device_path)
{
    videoCapture = new cv::VideoCapture(device_path);
    if (!videoCapture->isOpened())
    {
        qWarning() << "Cannot open camera.";
        isOpened = false;
    }
    else
    {
        isOpened = true;
    }
}

Camera::~Camera()
{
    videoCapture->release();

    delete videoCapture;
}

bool Camera::getFrame(cv::Mat &frame)
{
    if (isOpened)
    {
        return videoCapture->read(frame);
    }
    else
    {
        qWarning() << "Camera not opened.";
        return false;
    }
}
