#ifndef LATTE_CAMERA_H
#define LATTE_CAMERA_H

#include <opencv2/opencv.hpp>

class Camera
{
public:
    explicit Camera(const char *device_path);

    ~Camera();

    bool getFrame(cv::Mat &frame);

private:
    bool isOpened;
    cv::VideoCapture videoCapture;
};

#endif //LATTE_CAMERA_H
