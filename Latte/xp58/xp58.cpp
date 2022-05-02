#include <fcntl.h>
#include <unistd.h>
#include <string>

#include <QTextCodec>

#include "xp58.h"

Xp58::Xp58(const char *dev_path)
{
    fd = open(dev_path, O_RDWR);

    uint8_t buf[2] = {0x1B, '@'};
    write(fd, buf, 2);
}

Xp58::~Xp58()
{
    close(fd);
}

void Xp58::printLine(const QString &text) const
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    QByteArray data = (text + '\n').toLocal8Bit();
    write(fd, data, data.size());

    codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
}

void Xp58::feedNLines(uint8_t n) const
{
    uint8_t buf[2] = {0x1B, 'd'};
    write(fd, buf, 2);
    write(fd, &n, 1);
}

void Xp58::reverseModeOn() const
{
    uint8_t buf[3] = {0x1D, 'B', 1};
    write(fd, buf, 3);
}

void Xp58::reverseModeOff() const
{
    uint8_t buf[3] = {0x1D, 'B', 0};
    write(fd, buf, 3);
}

void Xp58::setCharacterSize(uint8_t width, uint8_t height) const
{
    uint8_t buf[2] = {0x1D, '!'};
    write(fd, buf, 2);

    uint8_t data = (width - 1) << 4 | (height - 1);
    write(fd, &data, 1);
}
