#ifndef LATTE_ROCKX_FACE_H
#define LATTE_ROCKX_FACE_H

#include <opencv2/opencv.hpp>
#include <rockx.h>

class RockxFace
{
public:
    explicit RockxFace();

    ~RockxFace();

    bool detect(cv::Mat &frame, rockx_object_array_t &face_array);

private:
    rockx_handle_t face_det_handle;
};

#endif //LATTE_ROCKX_FACE_H
