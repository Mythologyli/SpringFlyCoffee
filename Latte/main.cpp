#include "cxxopts/cxxopts.hpp"
#include "easyloggingpp/easylogging++.h"
#include "camera/camera.h"
#include "rockx_face/rockx_face.h"

INITIALIZE_EASYLOGGINGPP // NOLINT

int main(int argc, char **argv)
{
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime [%level]: %msg");

    cxxopts::Options options("Latte", "Coffee machine system on TB-RK3399ProD.");
    options.allow_unrecognised_options();
    options.add_options()
            ("c,camera", "Camera device path",
             cxxopts::value<std::string>()->default_value("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0"))
            ("h,help", "Help");

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    Camera camera(result["camera"].as<std::string>().c_str());
    RockxFace rockx_face;

    cv::Mat frame;
    camera.get_frame(frame);

    rockx_object_array_t face_array;
    memset(&face_array, 0, sizeof(rockx_object_array_t));

    rockx_face.detect(frame, face_array);

    for (int i = 0; i < face_array.count; i++)
    {
        LOG(INFO) << "Score: " << face_array.object[i].score;
    }

    return 0;
}
