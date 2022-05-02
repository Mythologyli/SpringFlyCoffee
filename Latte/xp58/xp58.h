#ifndef LATTE_XP58_H
#define LATTE_XP58_H

#include <QString>

class Xp58
{
public:
    explicit Xp58(const char *dev_path = "/dev/usb/lp0");

    ~Xp58();

    void printLine(const QString &text) const;

    void feedNLines(uint8_t n) const;

    void reverseModeOn() const;

    void reverseModeOff() const;

    void setCharacterSize(uint8_t width, uint8_t height) const;

private:
    int fd;
};

#endif //LATTE_XP58_H
