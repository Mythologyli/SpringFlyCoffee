#include "pump.h"

Pump::Pump(uint8_t pin_in1, uint8_t pin_in2)
{
    this->pin_in1 = pin_in1;
    this->pin_in2 = pin_in2;
}

void Pump::init()
{
    pinMode(pin_in1, OUTPUT);
    pinMode(pin_in2, OUTPUT);

    stop();
}

void Pump::start()
{
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, LOW);
}

void Pump::stop()
{   
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, HIGH);
}
