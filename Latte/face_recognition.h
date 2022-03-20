#ifndef LATTE_FACE_RECOGNITION_H
#define LATTE_FACE_RECOGNITION_H

#include <QObject>
#include <QtCore>

#include "camera/camera.h"
#include "rockx_face/rockx_face.h"

class FaceRecognition : public QObject
{
Q_OBJECT
public:
    explicit FaceRecognition(QObject *parent);

public slots:

    void save_face();

    void check_face();

private:
    bool get_feature(rockx_face_feature_t &feature);

    Camera camera;
    RockxFace rockx_face;

signals:

    void finished();
};

#endif //LATTE_FACE_RECOGNITION_H
