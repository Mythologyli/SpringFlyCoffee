#include <ctime>

#include "../easylogging++/easylogging++.h"

#include "rockx_face.h"

RockxFace::RockxFace()
{
    rockx_ret_t ret;

    face_det_handle = nullptr;
    ret = rockx_create(&face_det_handle, ROCKX_MODULE_FACE_DETECTION, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to create face_det_handle. Error: " << ret;
    }
}

bool RockxFace::detect(cv::Mat &frame, rockx_object_array_t &face_array)
{
    rockx_ret_t ret;

    rockx_image_t input_image;
    input_image.pixel_format = ROCKX_PIXEL_FORMAT_BGR888;
    input_image.width = frame.cols;
    input_image.height = frame.rows;
    input_image.data = frame.data;

    clock_t start_time, end_time;
    start_time = clock();

    ret = rockx_face_detect(face_det_handle, &input_image, &face_array, nullptr);

    end_time = clock();

    LOG(INFO) << "Detection time: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << "s";

    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to detect face. Error: " << ret;
        return false;
    }

    return true;
}

RockxFace::~RockxFace()
{
    rockx_destroy(face_det_handle);
}
