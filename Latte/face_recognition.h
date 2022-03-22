#ifndef LATTE_FACE_RECOGNITION_H
#define LATTE_FACE_RECOGNITION_H

#include <QObject>
#include <QtCore>

#include "rockx_face/rockx_face.h"

class FaceRecognition : public QObject
{
Q_OBJECT
public:
    explicit FaceRecognition(QObject *parent);

public slots:

    void save_face(rockx_image_t &input_image, const QString &name);

    void check_face(rockx_image_t &input_image, QString &name);

private:
    bool get_feature(rockx_image_t &input_image, rockx_face_feature_t &feature);

    RockxFace rockx_face;

signals:

    void save_face_succeed();

    void save_face_fail();

    void check_face_match();

    void check_face_no_match();

    void check_face_fail();
};

#endif //LATTE_FACE_RECOGNITION_H
