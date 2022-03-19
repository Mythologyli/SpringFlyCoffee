#ifndef ESPRESSO_PUMP_H
#define ESPRESSO_PUMP_H

#include <Arduino.h>

class Pump
{
public:
    explicit Pump(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_en);

    void init();

    void start();

    void stop();

private:
    uint8_t pin_in1;
    uint8_t pin_in2;
    uint8_t pin_en;
};

#endif
