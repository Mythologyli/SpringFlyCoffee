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
    WebSocketsClient client("ws://10.112.193.132:9876", &app);

    QObject::connect(&client, &WebSocketsClient::command_check_received,
                     [&]()
                     {
                         cv::Mat frame;
                         camera.get_frame(frame);

                         rockx_image_t input_image;
                         RockxFace::cv_mat_to_rockx_image(frame, input_image);

                         QString name;

                         face_recognition.check_face(input_image, name);
                         qInfo() << "Check result:" << name;
                     });
    QObject::connect(&client, &WebSocketsClient::command_save_received,
                     [&]()
                     {
                         cv::Mat frame;
                         camera.get_frame(frame);

                         rockx_image_t input_image;
                         RockxFace::cv_mat_to_rockx_image(frame, input_image);

                         QDateTime date = QDateTime::currentDateTime();
                         QString time_str = date.toString("yyyyddMMhhmmss");

                         face_recognition.save_face(input_image, time_str);
                     });

    QObject::connect(&face_recognition, &FaceRecognition::finished,
                     &client, &WebSocketsClient::close);
    QObject::connect(&face_recognition, &FaceRecognition::finished,
                     &app, &QCoreApplication::quit);

    QTimer::singleShot(10, &app,
                       [&]()
                       {
                           if (parser.isSet("save"))
                           {
                               client.send("save");
                           }
                           else
                           {
                               client.send("check");
                           }
                       });

    return QCoreApplication::exec();
}
