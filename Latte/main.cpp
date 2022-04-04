#include <QApplication>
#include <QCommandLineParser>

#include "facewindow/facewindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Latte");
    QApplication::setApplicationVersion("0.1");

    QString messagePattern = "%{time yyyyMMdd h:mm:ss.zzz} ";
    messagePattern += "%{if-debug}[Debug] %{file}:%{line}%{endif}";
    messagePattern += "%{if-info}[Info]%{endif}";
    messagePattern += "%{if-warning}[Warning] %{file}:%{line}%{endif}";
    messagePattern += "%{if-critical}[Critical] %{file}:%{line}%{endif}";
    messagePattern += "%{if-fatal}[Fatal] %{file}:%{line}%{endif}";
    messagePattern += " %{message}";

    qSetMessagePattern(messagePattern);

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

    FaceWindow faceWindow;
    if (parser.isSet("save"))
    {
        faceWindow.setMode(FaceWindow::Mode::Save);
    }
    else
    {
        faceWindow.setMode(FaceWindow::Mode::Check);
    }

    faceWindow.showFullScreen();

    return QApplication::exec();
}
