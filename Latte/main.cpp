#include <iostream>

#include <opencv2/opencv.hpp>
#include <rockx.h>

int main()
{
    // Get image from camera.
    cv::VideoCapture cap("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0");
    if (!cap.isOpened())
    {
        std::cout << "Cannot open camera. Remember to run this program as root user." << std::endl;
        return 1;
    }

    cv::Mat frame;
    cap.read(frame);

    // Face detection using RockX.
    rockx_ret_t ret;
    rockx_handle_t face_det_handle;

    rockx_object_array_t face_array;
    memset(&face_array, 0, sizeof(rockx_object_array_t));

    ret = rockx_create(&face_det_handle, ROCKX_MODULE_FACE_DETECTION, nullptr, 0);
    if (ret != ROCKX_RET_SUCCESS)
    {
        std::cout << "Fail to create face detector. Error: " << ret << std::endl;
        return -1;
    }

    rockx_image_t input_image;
    input_image.pixel_format = ROCKX_PIXEL_FORMAT_BGR888;
    input_image.width = frame.cols;
    input_image.height = frame.rows;
    input_image.data = frame.data;

    ret = rockx_face_detect(face_det_handle, &input_image, &face_array, nullptr);
    if (ret != ROCKX_RET_SUCCESS)
    {
        std::cout << "Fail to detect face. Error: " << ret << std::endl;
        return -1;
    }

    for (int i = 0; i < face_array.count; i++)
    {
        int left = face_array.object[i].box.left;
        int top = face_array.object[i].box.top;
        int right = face_array.object[i].box.right;
        int bottom = face_array.object[i].box.bottom;
        float score = face_array.object[i].score;
        std::cout << "Score: " << score << std::endl;

        char score_str[8];
        memset(score_str, 0, 8);
        snprintf(score_str, 8, "%.3f", score);
        rockx_image_draw_rect(&input_image, {left, top}, {right, bottom}, {0, 255, 0}, 3);
        rockx_image_draw_text(&input_image, score_str, {left, top - 12}, {0, 255, 0}, 3);
    }

    // Save result.
    rockx_image_write("./out.jpg", &input_image);

    rockx_destroy(face_det_handle);

    return 0;
}
