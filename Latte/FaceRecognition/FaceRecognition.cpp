#include <QDebug>
#include <QFile>
#include <QDir>

#include "FaceRecognition.h"

FaceRecognition::FaceRecognition(QObject *parent) : QObject(parent)
{
    rockxFace = new RockxFace();

    rockxFace->setSimilarityThreshold(0.3);
}

FaceRecognition::~FaceRecognition()
{
    delete rockxFace;
}

bool FaceRecognition::getFeature(rockx_image_t &inputImage, rockx_face_feature_t &feature)
{
    rockx_object_array_t faceArray;
    memset(&faceArray, 0, sizeof(rockx_object_array_t));

    if (!rockxFace->detect(inputImage, faceArray))
    {
        return false;
    }

    if (faceArray.count == 0)
    {
        qInfo() << "No faces detected.";
        return false;
    }

    for (int i = 0; i < faceArray.count; i++)
    {
        qInfo() << "Score:" << faceArray.object[i].score;
    }

    emit faceBoxGetted(faceArray.object[0].box);

    rockx_image_t outputImage;
    memset(&outputImage, 0, sizeof(rockx_image_t));

    if (!rockxFace->align(inputImage, faceArray.object[0], outputImage))
    {
        return false;
    }

    rockxFace->recognize(outputImage, feature);

    rockx_image_release(&outputImage);

    return true;
}

void FaceRecognition::saveFace(rockx_image_t &inputImage, const QString &name)
{
    rockx_face_feature_t feature;
    memset(&feature, 0, sizeof(rockx_face_feature_t));

    if (!getFeature(inputImage, feature))
    {
        emit saveFaceFail();
        return;
    }

    QDir dir;
    dir.mkdir("faces");
    QFile file("faces/" + name + ".feature");
    file.open(QIODevice::WriteOnly);
    file.write((const char *) (&feature), sizeof(rockx_face_feature_t));
    file.close();

    qInfo() << "Save feature to faces/" + name + ".feature";

    emit saveFaceSucceed();
}

void FaceRecognition::checkFace(rockx_image_t &inputImage, QString &name)
{
    rockx_face_feature_t feature;
    memset(&feature, 0, sizeof(rockx_face_feature_t));

    if (!getFeature(inputImage, feature))
    {
        emit checkFaceFail();
        return;
    }

    rockx_face_feature_t originFeature;
    memset(&originFeature, 0, sizeof(rockx_face_feature_t));

    QDir dir("faces");
    QStringList nameFilters;
    nameFilters << "*.feature";
    QStringList nameList = dir.entryList(nameFilters, QDir::Files | QDir::Readable);

    int minIndex = 0;
    float minSimilarity = 100.0;
    for (int i = 0; i < nameList.count(); i++)
    {
        QFile file("faces/" + nameList[i]);
        file.open(QIODevice::ReadOnly);
        file.read((char *) (&originFeature), sizeof(rockx_face_feature_t));

        float similarity = RockxFace::compare(feature, originFeature);
        if (similarity < minSimilarity)
        {
            minSimilarity = similarity;
            minIndex = i;
        }

        file.close();
    }

    if (minSimilarity <= rockxFace->getSimilarityThreshold())
    {
        name = nameList[minIndex].split(".")[0];
        qInfo() << "Face is same as" << name;

        emit checkFaceMatch();
    }
    else
    {
        name = "";
        qInfo() << "Face is not same as any face.";

        emit checkFaceNoMatch();
    }
}
