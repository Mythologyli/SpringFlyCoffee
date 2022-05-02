#ifndef LATTE_FACERECOGNITION_H
#define LATTE_FACERECOGNITION_H

#include <QObject>

#include "../rockxface/rockxface.h"

class FaceRecognition : public QObject
{
Q_OBJECT
public:
    explicit FaceRecognition(QObject *parent);

    ~FaceRecognition() override;

public slots:

    void saveFace(rockx_image_t &inputImage, const QString &name);

    void checkFace(rockx_image_t &inputImage, QString &name);

private slots:

    bool getFeature(rockx_image_t &inputImage, rockx_face_feature_t &feature);

private:
    RockxFace *rockxFace;

signals:

    void faceBoxGet(rockx_rect_t box);

    void noFaceFound();

    void faceSaved();

    void faceMatched(const QString& name);

    void faceNoMatched();
};

#endif //LATTE_FACERECOGNITION_H
