#include <QObject>
#include <QtCore>

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

    FaceRecognition face_recognition(&app);
    WebSocketsClient client("ws://10.112.193.132:9876", &app);

    QObject::connect(&client, &WebSocketsClient::command_check_received,
                     &face_recognition, &FaceRecognition::check_face);
    QObject::connect(&client, &WebSocketsClient::command_save_received,
                     &face_recognition, &FaceRecognition::save_face);

    QObject::connect(&face_recognition, &FaceRecognition::finished,
                     &client, &WebSocketsClient::close);
    QObject::connect(&face_recognition, &FaceRecognition::finished,
                     &app, &QCoreApplication::quit);

    QTimer timer(&app);
    QObject::connect(&timer, &QTimer::timeout, [&client, &parser]()
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
    timer.start(1000);

    return QCoreApplication::exec();
}
