#ifndef LATTE_ROCKXFACE_H
#define LATTE_ROCKXFACE_H

#include <opencv2/opencv.hpp>
#include <rockx.h>

class RockxFace
{
public:
    explicit RockxFace();

    ~RockxFace();

    bool detect(rockx_image_t &inputImage, rockx_object_array_t &faceArray);

    bool align(rockx_image_t &inputImage, rockx_object_t &face, rockx_image_t &outputImage);

    bool recognize(rockx_image_t &inputImage, rockx_face_feature_t &feature);

    static float compare(rockx_face_feature_t &feature1, rockx_face_feature_t &feature2);

    bool isFaceSame(rockx_face_feature_t &feature1, rockx_face_feature_t &feature2) const;

    float getSimilarityThreshold() const;

    void setSimilarityThreshold(float threshold);

    static void cvMatToRockxImage(cv::Mat &frame, rockx_image_t &image);

private:
    rockx_handle_t faceDetectionHandle;
    rockx_handle_t faceLandmarkHandle;
    rockx_handle_t faceRecognizeHandle;

    float similarityThreshold;
};

#endif //LATTE_ROCKXFACE_H
