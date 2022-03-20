#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include <QObject>
#include <QtCore>

#include "serial/serial.h"
#include "camera/camera.h"
#include "rockx_face/rockx_face.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Latte");
    QCoreApplication::setApplicationVersion("0.1");

    QString message_pattern = "%{time yyyyMMdd h:mm:ss.zzz} ";
    message_pattern += "%{if-debug}[Debug] %{file}:%{line}%{endif}";
    message_pattern += "%{if-info}[Info]%{endif}";
    message_pattern += "%{if-warning}[Warning] %{file}:%{line}%{endif}";
    message_pattern += "%{if-critical}[Critical] %{file}:%{line}%{endif}";
    message_pattern += "%{if-fatal}[Fatal] %{file}:%{line}%{endif}";
    message_pattern += " %{message}";

    qSetMessagePattern(message_pattern);

    QCommandLineParser parser;
    parser.setApplicationDescription("Coffee machine system on TB-RK3399ProD.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
                              {
                                      {"c", "camera"},
                                      QCoreApplication::translate("main", "Camera device <path>."),
                                      QCoreApplication::translate("main", "path")
                              },
                              {
                                      {"s", "save"},
                                      QCoreApplication::translate("main", "Save feature to origin.feature.")
                              }
                      }
    );

    parser.process(app);

    QString camera_device_path = parser.value("camera");
    Camera camera(camera_device_path.isEmpty() ? "/dev/v4l/by-id/usb-Generic_HD_camera-video-index0"
                                               : camera_device_path.toLocal8Bit());
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
        qInfo() << "No faces detected.";
        return 0;
    }

    for (int i = 0; i < face_array.count; i++)
    {
        qInfo() << "Score:" << face_array.object[i].score;
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

    if (parser.isSet("save"))
    {
        int fd = open("origin.feature", O_WRONLY | O_CREAT);
        write(fd, &feature, sizeof(rockx_face_feature_t));
        close(fd);

        qInfo() << "Save feature to origin.feature.";
    }
    else
    {
        rockx_face_feature_t origin_feature;
        memset(&origin_feature, 0, sizeof(rockx_face_feature_t));

        int fd = open("origin.feature", O_RDONLY);
        read(fd, &origin_feature, sizeof(rockx_face_feature_t));
        close(fd);

        if (rockx_face.is_face_same(feature, origin_feature))
        {
            qInfo() << "Face is same as origin face.";
        }
        else
        {
            qInfo() << "Face is not same as origin face.";
        }
    }

    return 0;
}
