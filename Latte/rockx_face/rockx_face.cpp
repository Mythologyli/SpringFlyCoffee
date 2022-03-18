#include <ctime>

#include "../easylogging++/easylogging++.h"

#include "rockx_face.h"

RockxFace::RockxFace()
{
    rockx_ret_t ret;

    face_detection_handle = nullptr;
    ret = rockx_create(&face_detection_handle, ROCKX_MODULE_FACE_DETECTION, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to create face_detection_handle. Error: " << ret;
    }

    face_landmark_handle = nullptr;
    ret = rockx_create(&face_landmark_handle, ROCKX_MODULE_FACE_LANDMARK_5, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to create face_landmark_handle. Error: " << ret;
    }

    face_recognize_handle = nullptr;
    ret = rockx_create(&face_recognize_handle, ROCKX_MODULE_FACE_RECOGNIZE, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to create face_recognize_handle. Error: " << ret;
    }
}

bool RockxFace::detect(rockx_image_t &input_image, rockx_object_array_t &face_array)
{
    rockx_ret_t ret;

    clock_t start_time, end_time;
    start_time = clock();

    ret = rockx_face_detect(face_detection_handle, &input_image, &face_array, nullptr);

    end_time = clock();

    LOG(INFO) << "Detect time: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << "s";

    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to detect face. Error: " << ret;
        return false;
    }

    return true;
}

bool RockxFace::align(rockx_image_t &input_image, rockx_object_t &face, rockx_image_t &output_image)
{
    rockx_ret_t ret;

    clock_t start_time, end_time;
    start_time = clock();

    ret = rockx_face_align(face_landmark_handle, &input_image, &(face.box), nullptr, &output_image);

    end_time = clock();

    LOG(INFO) << "Align time: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << "s";

    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to align face. Error: " << ret;
        return false;
    }

    return true;
}

bool RockxFace::recognize(rockx_image_t &input_image, rockx_face_feature_t &feature)
{
    rockx_ret_t ret;

    clock_t start_time, end_time;
    start_time = clock();

    ret = rockx_face_recognize(face_recognize_handle, &input_image, &feature);

    end_time = clock();

    LOG(INFO) << "Recognize time: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << "s";

    if (ret != ROCKX_RET_SUCCESS)
    {
        LOG(ERROR) << "Fail to recognize face. Error: " << ret;
        return false;
    }

    return true;
}

RockxFace::~RockxFace()
{
    rockx_destroy(face_detection_handle);
    rockx_destroy(face_landmark_handle);
    rockx_destroy(face_recognize_handle);
}
