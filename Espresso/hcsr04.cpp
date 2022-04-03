#include "hcsr04.h"

HcSr04::HcSr04(uint8_t trigPin, uint8_t echoPin)
{
    this->trigPin = trigPin;
    this->echoPin = echoPin;
}

void HcSr04::init()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(trigPin, LOW);
}

float HcSr04::getDistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    return static_cast<float>(pulseIn(echoPin, HIGH)) * 0.034 / 2.0;
}
