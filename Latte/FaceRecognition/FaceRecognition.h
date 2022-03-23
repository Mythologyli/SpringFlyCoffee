#ifndef LATTE_FACERECOGNITION_H
#define LATTE_FACERECOGNITION_H

#include <QObject>

#include "../RockxFace/RockxFace.h"

class FaceRecognition : public QObject
{
Q_OBJECT
public:
    explicit FaceRecognition(QObject *parent);

    ~FaceRecognition() override;

public slots:

    void saveFace(rockx_image_t &inputImage, const QString &name);

    void checkFace(rockx_image_t &inputImage, QString &name);

private:
    bool getFeature(rockx_image_t &inputImage, rockx_face_feature_t &feature);

    RockxFace *rockxFace;

signals:

    void faceBoxGetted(rockx_rect_t box);

    void saveFaceSucceed();

    void saveFaceFail();

    void checkFaceMatch();

    void checkFaceNoMatch();

    void checkFaceFail();
};

#endif //LATTE_FACERECOGNITION_H
