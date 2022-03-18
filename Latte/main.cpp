#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "cxxopts/cxxopts.hpp"
#include "easylogging++/easylogging++.h"
#include "camera/camera.h"
#include "rockx_face/rockx_face.h"

INITIALIZE_EASYLOGGINGPP // NOLINT

int main(int argc, char **argv)
{
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime [%level]: %msg");

    cxxopts::Options cxxopts_options("Latte", "Coffee machine system on TB-RK3399ProD.");
    cxxopts_options.allow_unrecognised_options();
    cxxopts_options.add_options()
            ("c,camera", "Camera device path",
             cxxopts::value<std::string>()->default_value("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0"))
            ("s,save", "Save feature to origin.feature")
            ("h,help", "Help");

    auto cxxopts_result = cxxopts_options.parse(argc, argv);

    if (cxxopts_result.count("help"))
    {
        std::cout << cxxopts_options.help() << std::endl;
        return 0;
    }

    Camera camera((const char *) cxxopts_result["camera"].as<std::string>().c_str());
    RockxFace rockx_face;
    rockx_face.set_similarity_threshold(0.5);

    cv::Mat frame;
    if (!camera.get_frame(frame))
    {
        return 0;
    }

    rockx_image_t input_image;
    input_image.pixel_format = ROCKX_PIXEL_FORMAT_BGR888;
    input_image.width = frame.cols;
    input_image.height = frame.rows;
    input_image.data = frame.data;

    rockx_object_array_t face_array;
    memset(&face_array, 0, sizeof(rockx_object_array_t));

    if (!rockx_face.detect(input_image, face_array))
    {
        return 0;
    }

    if (face_array.count == 0)
    {
        LOG(INFO) << "No faces detected.";
        return 0;
    }

    for (int i = 0; i < face_array.count; i++)
    {
        LOG(INFO) << "Score: " << face_array.object[i].score;
    }

    rockx_image_t output_image;
    memset(&output_image, 0, sizeof(rockx_image_t));

    if (!rockx_face.align(input_image, face_array.object[0], output_image))
    {
        return 0;
    }

    rockx_face_feature_t feature;
    memset(&feature, 0, sizeof(rockx_face_feature_t));

    if (!rockx_face.recognize(output_image, feature))
    {
        return 0;
    }

    rockx_image_release(&output_image);

    if (cxxopts_result.count("save"))
    {
        int fd = open("origin.feature", O_WRONLY | O_CREAT);
        write(fd, &feature, sizeof(rockx_face_feature_t));
        close(fd);

        LOG(INFO) << "Save feature to origin.feature.";
    }
    else
    {
        rockx_face_feature_t origin_feature;
        memset(&origin_feature, 0, sizeof(rockx_face_feature_t));

        int fd = open("origin.feature", O_RDONLY);
        read(fd, &origin_feature, sizeof(rockx_face_feature_t));
        close(fd);

        if (rockx_face.is_feature_same(feature, origin_feature))
        {
            LOG(INFO) << "Feature is same as origin feature.";
        }
        else
        {
            LOG(INFO) << "Feature is not same as origin feature.";
        }
    }

    return 0;
}
