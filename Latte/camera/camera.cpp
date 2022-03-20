#include <QtCore>

#include "camera.h"

Camera::Camera(const char *device_path)
{
    video_capture = cv::VideoCapture(device_path);
    if (!video_capture.isOpened())
    {
        qWarning() << "Cannot open camera. Remember to run this program as root user.";
        is_opened = false;
    }
    else
    {
        is_opened = true;
    }
}

bool Camera::get_frame(cv::Mat &frame)
{
    if (is_opened)
    {
        return video_capture.read(frame);
    }
    else
    {
        qWarning() << "Camera not opened.";
        return false;
    }
}

Camera::~Camera()
{
    video_capture.release();
}
