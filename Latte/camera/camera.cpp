#include "../easyloggingpp/easylogging++.h"

#include "camera.h"

Camera::Camera(const char *device_path)
{
    video_capture = cv::VideoCapture(device_path);
    if (!video_capture.isOpened())
    {
        LOG(ERROR) << "Cannot open camera. Remember to run this program as root user.";
        is_opened = false;
    }

    is_opened = true;
}

bool Camera::get_frame(cv::Mat &frame)
{
    if (is_opened)
    {
        return video_capture.read(frame);
    }
    else
    {
        LOG(ERROR) << "Camera not opened.";
        return false;
    }
}
