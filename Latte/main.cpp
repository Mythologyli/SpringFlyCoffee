#include <QWidget>
#include <QApplication>
#include <QLabel>

#include "camera/camera.h"
#include "face_recognition.h"
#include "websockets_client.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Latte");
    QApplication::setApplicationVersion("0.1");

    QWidget window;
    window.setStyleSheet("background-color: black;");

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
                                      QApplication::translate("main", "Save feature to origin.feature.")
                              }
                      }
    );

    parser.process(app);

    Camera camera("/dev/v4l/by-id/usb-Generic_HD_camera-video-index0");
    FaceRecognition face_recognition(&app);

    QLabel label(&window);
    label.setGeometry(100, 0, 600, 480);

    QObject::connect(&face_recognition, &FaceRecognition::save_face_succeed,
                     &app, &QApplication::quit);
    QObject::connect(&face_recognition, &FaceRecognition::check_face_match,
                     &app, &QApplication::quit);

    QTimer timer(&window);
    QObject::connect(&timer, &QTimer::timeout,
                     [&]()
                     {
                         cv::Mat frame;
                         cv::Mat converted_frame;
                         camera.get_frame(frame);
                         cv::cvtColor(frame, converted_frame, CV_BGR2RGB);

                         QImage image((uchar *) converted_frame.data,
                                      converted_frame.cols,
                                      converted_frame.rows,
                                      (int) converted_frame.step,
                                      QImage::Format_RGB888);

                         label.setPixmap(QPixmap::fromImage(image));

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
    timer.start(80);

    window.showFullScreen();

    return QApplication::exec();
}
