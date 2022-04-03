#ifndef ESPRESSO_PUMP_H
#define ESPRESSO_PUMP_H

#include <Arduino.h>

class Pump
{
public:
    explicit Pump(uint8_t in1Pin, uint8_t in2Pin);

    void init();

    void start();

    void stop();

private:
    uint8_t in1Pin;
    uint8_t in2Pin;
};

#endif // ESPRESSO_PUMP_H
