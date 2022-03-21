#include <QFile>

#include "face_recognition.h"

FaceRecognition::FaceRecognition(QObject *parent) : QObject(parent),
                                                    camera("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0")
{
    rockx_face.set_similarity_threshold(0.5);
}

bool FaceRecognition::get_feature(rockx_face_feature_t &feature)
{
    cv::Mat frame;
    if (!camera.get_frame(frame))
    {
        return false;
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
        return false;
    }

    if (face_array.count == 0)
    {
        qInfo() << "No faces detected.";
        return false;
    }

    for (int i = 0; i < face_array.count; i++)
    {
        qInfo() << "Score:" << face_array.object[i].score;
    }

    rockx_image_t output_image;
    memset(&output_image, 0, sizeof(rockx_image_t));

    if (!rockx_face.align(input_image, face_array.object[0], output_image))
    {
        return false;
    }

    rockx_face.recognize(output_image, feature);

    rockx_image_release(&output_image);

    return true;
}

void FaceRecognition::save_face()
{
    rockx_face_feature_t feature;
    memset(&feature, 0, sizeof(rockx_face_feature_t));

    if (!get_feature(feature))
    {
        emit finished();
        return;
    }

    QFile file("origin.feature");
    file.open(QIODevice::WriteOnly);
    file.write((const char *) (&feature), sizeof(rockx_face_feature_t));
    file.close();

    qInfo() << "Save feature to origin.feature.";

    emit finished();
}

void FaceRecognition::check_face()
{
    rockx_face_feature_t feature;
    memset(&feature, 0, sizeof(rockx_face_feature_t));

    if (!get_feature(feature))
    {
        emit finished();
        return;
    }

    rockx_face_feature_t origin_feature;
    memset(&origin_feature, 0, sizeof(rockx_face_feature_t));

    QFile file("origin.feature");
    file.open(QIODevice::ReadOnly);
    file.read((char *) (&feature), sizeof(rockx_face_feature_t));
    file.close();

    if (rockx_face.is_face_same(feature, origin_feature))
    {
        qInfo() << "Face is same as origin face.";
    }
    else
    {
        qInfo() << "Face is not same as origin face.";
    }

    emit finished();
}
