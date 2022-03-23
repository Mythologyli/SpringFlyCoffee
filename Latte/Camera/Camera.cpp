#include <QDebug>

#include "Camera.h"

Camera::Camera(const char *device_path)
{
    videoCapture = cv::VideoCapture(device_path);
    if (!videoCapture.isOpened())
    {
        qWarning() << "Cannot open camera.";
        isOpened = false;
    }
    else
    {
        isOpened = true;
    }
}

bool Camera::getFrame(cv::Mat &frame)
{
    if (isOpened)
    {
        return videoCapture.read(frame);
    }
    else
    {
        qWarning() << "Camera not opened.";
        return false;
    }
}

Camera::~Camera()
{
    videoCapture.release();
}
