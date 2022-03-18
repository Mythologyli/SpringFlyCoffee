#ifndef LATTE_CAMERA_H
#define LATTE_CAMERA_H

#include <opencv2/opencv.hpp>

class Camera
{
public:
    explicit Camera(const char *device_path);

    bool get_frame(cv::Mat &frame);

private:
    bool is_opened;
    cv::VideoCapture video_capture;
};

#endif //LATTE_CAMERA_H
