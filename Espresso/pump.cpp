#include "pump.h"

Pump::Pump(uint8_t in1Pin, uint8_t in2Pin)
{
    this->in1Pin = in1Pin;
    this->in2Pin = in2Pin;
}

void Pump::init()
{
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);

    stop();
}

void Pump::start()
{
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
}

void Pump::stop()
{   
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, HIGH);
}
