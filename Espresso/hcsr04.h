#ifndef ESPRESSO_HCSR04_H
#define ESPRESSO_HCSR04_H

#include <Arduino.h>

class HcSr04
{
public:
    explicit HcSr04(uint8_t trigPin, uint8_t pin_echo);

    void init();

    float getDistance();

private:
    uint8_t trigPin;
    uint8_t echoPin;
};

#endif // ESPRESSO_HCSR04_H
