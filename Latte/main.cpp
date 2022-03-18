#include "easyloggingpp/easylogging++.h"
#include "camera/camera.h"
#include "rockx_face/rockx_face.h"

INITIALIZE_EASYLOGGINGPP // NOLINT

int main()
{
    Camera camera("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0");
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
