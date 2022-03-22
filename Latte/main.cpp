#include <QObject>
#include <QtCore>

#include "camera/camera.h"
#include "face_recognition.h"
#include "websockets_client.h"

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
                                      {"s", "save"},
                                      QCoreApplication::translate("main", "Save feature to origin.feature.")
                              }
                      }
    );

    parser.process(app);

    Camera camera("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0");
    FaceRecognition face_recognition(&app);

    QObject::connect(&face_recognition, &FaceRecognition::finished,
                     &app, &QCoreApplication::quit);

    QTimer::singleShot(10, &app,
                       [&]()
                       {
                           cv::Mat frame;
                           camera.get_frame(frame);

                           rockx_image_t input_image;
                           RockxFace::cv_mat_to_rockx_image(frame, input_image);

                           if (parser.isSet("save"))
                           {
                               QDateTime date = QDateTime::currentDateTime();
                               QString time_str = date.toString("yyyyddMMhhmmss");

                               face_recognition.save_face(input_image, time_str);
                           }
                           else
                           {
                               QString name;

                               face_recognition.check_face(input_image, name);
                               qInfo() << "Check result:" << name;
                           }
                       });

    return QCoreApplication::exec();
}
