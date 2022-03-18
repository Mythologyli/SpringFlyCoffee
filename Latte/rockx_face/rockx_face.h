#ifndef LATTE_ROCKX_FACE_H
#define LATTE_ROCKX_FACE_H

#include <rockx.h>

class RockxFace
{
public:
    explicit RockxFace();

    ~RockxFace();

    bool detect(rockx_image_t &input_image, rockx_object_array_t &face_array);

    bool align(rockx_image_t &input_image, rockx_object_t &face, rockx_image_t &output_image);

    bool recognize(rockx_image_t &input_image, rockx_face_feature_t &feature);

    bool is_feature_same(rockx_face_feature_t &feature1, rockx_face_feature_t &feature2) const;

    float get_similarity_threshold() const;

    void set_similarity_threshold(float threshold);

private:
    rockx_handle_t face_detection_handle;
    rockx_handle_t face_landmark_handle;
    rockx_handle_t face_recognize_handle;

    float similarity_threshold;
};

#endif //LATTE_ROCKX_FACE_H
