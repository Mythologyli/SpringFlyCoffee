#ifndef LATTE_ROCKX_FACE_H
#define LATTE_ROCKX_FACE_H

#include <opencv2/opencv.hpp>
#include <rockx.h>

class RockxFace
{
public:
    explicit RockxFace();

    ~RockxFace();

    bool detect(rockx_image_t &input_image, rockx_object_array_t &face_array);

    bool align(rockx_image_t &input_image, rockx_object_t &face, rockx_image_t &output_image);

    bool recognize(rockx_image_t &input_image, rockx_face_feature_t &feature);

    static float compare(rockx_face_feature_t &feature1, rockx_face_feature_t &feature2);

    bool is_face_same(rockx_face_feature_t &feature1, rockx_face_feature_t &feature2) const;

    float get_similarity_threshold() const;

    void set_similarity_threshold(float threshold);

    static void cv_mat_to_rockx_image(cv::Mat &frame, rockx_image_t &image);

private:
    rockx_handle_t face_detection_handle;
    rockx_handle_t face_landmark_handle;
    rockx_handle_t face_recognize_handle;

    float similarity_threshold;
};

#endif //LATTE_ROCKX_FACE_H
