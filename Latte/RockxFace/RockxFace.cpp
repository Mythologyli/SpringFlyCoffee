#include <ctime>

#include <QDebug>

#include "RockxFace.h"

RockxFace::RockxFace()
{
    rockx_ret_t ret;

    similarityThreshold = 1.0;

    faceDetectionHandle = nullptr;
    ret = rockx_create(&faceDetectionHandle, ROCKX_MODULE_FACE_DETECTION, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        qWarning() << "Fail to create faceDetectionHandle. Error:" << ret;
    }

    faceLandmarkHandle = nullptr;
    ret = rockx_create(&faceLandmarkHandle, ROCKX_MODULE_FACE_LANDMARK_5, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        qWarning() << "Fail to create faceLandmarkHandle. Error:" << ret;
    }

    faceRecognizeHandle = nullptr;
    ret = rockx_create(&faceRecognizeHandle, ROCKX_MODULE_FACE_RECOGNIZE, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        qWarning() << "Fail to create faceRecognizeHandle. Error:" << ret;
    }
}

bool RockxFace::detect(rockx_image_t &inputImage, rockx_object_array_t &faceArray)
{
    rockx_ret_t ret;

    clock_t startTime, endTime;
    startTime = clock();

    ret = rockx_face_detect(faceDetectionHandle, &inputImage, &faceArray, nullptr);

    endTime = clock();

    qInfo() << "Detect time:" << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s";

    if (ret != ROCKX_RET_SUCCESS)
    {
        qWarning() << "Fail to detect face. Error:" << ret;
        return false;
    }

    return true;
}

bool RockxFace::align(rockx_image_t &inputImage, rockx_object_t &face, rockx_image_t &outputImage)
{
    rockx_ret_t ret;

    clock_t startTime, endTime;
    startTime = clock();

    ret = rockx_face_align(faceLandmarkHandle, &inputImage, &(face.box), nullptr, &outputImage);

    endTime = clock();

    qInfo() << "Align time:" << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s";

    if (ret != ROCKX_RET_SUCCESS)
    {
        qWarning() << "Fail to align face. Error:" << ret;
        return false;
    }

    return true;
}

bool RockxFace::recognize(rockx_image_t &inputImage, rockx_face_feature_t &feature)
{
    rockx_ret_t ret;

    clock_t startTime, endTime;
    startTime = clock();

    ret = rockx_face_recognize(faceRecognizeHandle, &inputImage, &feature);

    endTime = clock();

    qInfo() << "Recognize time:" << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s";

    if (ret != ROCKX_RET_SUCCESS)
    {
        qWarning() << "Fail to recognize face. Error:" << ret;
        return false;
    }

    return true;
}

float RockxFace::getSimilarityThreshold() const
{
    return similarityThreshold;
}

void RockxFace::setSimilarityThreshold(float threshold)
{
    similarityThreshold = threshold;
}

float RockxFace::compare(rockx_face_feature_t &feature1, rockx_face_feature_t &feature2)
{
    float similarity;
    rockx_face_feature_similarity(&feature1, &feature2, &similarity);

    qInfo() << "Similarity:" << similarity;

    return similarity;
}

bool RockxFace::isFaceSame(rockx_face_feature_t &feature1, rockx_face_feature_t &feature2) const
{
    return compare(feature1, feature2) <= similarityThreshold;
}

void RockxFace::cvMatToRockxImage(cv::Mat &frame, rockx_image_t &image)
{
    image.pixel_format = ROCKX_PIXEL_FORMAT_BGR888;
    image.width = frame.cols;
    image.height = frame.rows;
    image.data = frame.data;
}

RockxFace::~RockxFace()
{
    rockx_destroy(faceDetectionHandle);
    rockx_destroy(faceLandmarkHandle);
    rockx_destroy(faceRecognizeHandle);
}
