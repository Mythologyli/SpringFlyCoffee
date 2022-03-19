#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPushButton button("Hello world!", nullptr);
    button.setFont(QFont("Times", 40));
    button.resize(800, 480);
    button.show();

    return QApplication::exec();
}
