#include <QFile>
#include <QDir>

#include "face_recognition.h"

FaceRecognition::FaceRecognition(QObject *parent) : QObject(parent)
{
    rockx_face.set_similarity_threshold(0.5);
}

bool FaceRecognition::get_feature(rockx_image_t &input_image, rockx_face_feature_t &feature)
{
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

void FaceRecognition::save_face(rockx_image_t &input_image, const QString &name)
{
    rockx_face_feature_t feature;
    memset(&feature, 0, sizeof(rockx_face_feature_t));

    if (!get_feature(input_image, feature))
    {
        emit save_face_fail();
        return;
    }

    QDir dir;
    dir.mkdir("faces");
    QFile file("faces/" + name + ".feature");
    file.open(QIODevice::WriteOnly);
    file.write((const char *) (&feature), sizeof(rockx_face_feature_t));
    file.close();

    qInfo() << "Save feature to faces/" + name + ".feature";

    emit save_face_succeed();
}

void FaceRecognition::check_face(rockx_image_t &input_image, QString &name)
{
    rockx_face_feature_t feature;
    memset(&feature, 0, sizeof(rockx_face_feature_t));

    if (!get_feature(input_image, feature))
    {
        emit check_face_fail();
        return;
    }

    rockx_face_feature_t origin_feature;
    memset(&origin_feature, 0, sizeof(rockx_face_feature_t));

    QDir dir("faces");
    QStringList name_filters;
    name_filters << "*.feature";
    QStringList filename_list = dir.entryList(name_filters, QDir::Files | QDir::Readable);

    int min_i = 0;
    float min_similarity = 100.0;
    for (int i = 0; i < filename_list.count(); i++)
    {
        QFile file("faces/" + filename_list[i]);
        file.open(QIODevice::ReadOnly);
        file.read((char *) (&origin_feature), sizeof(rockx_face_feature_t));

        float similarity = RockxFace::compare(feature, origin_feature);
        if (similarity < min_similarity)
        {
            min_similarity = similarity;
            min_i = i;
        }

        file.close();
    }

    if (min_similarity <= rockx_face.get_similarity_threshold())
    {
        name = filename_list[min_i].split(".")[0];
        qInfo() << "Face is same as" << name;

        emit check_face_match();
    }
    else
    {
        name = "";
        qInfo() << "Face is not same as any face.";

        emit check_face_no_match();
    }
}
