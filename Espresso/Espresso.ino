#include <Arduino.h>

#include "pump.h"
#include "hcsr04.h"

#define SERIAL_BAUD 115200

Pump pump1(2, 3);
Pump pump2(4, 5);
HcSr04 hcsr04(6, 7);

float distance = 0.0;

void setup()
{
    pump1.init();
    pump2.init();

    hcsr04.init();

    Serial.begin(SERIAL_BAUD);
}

void loop()
{
    float currentDistance = hcsr04.getDistance();
    if (distance != currentDistance)
    {
        distance = currentDistance;
        Serial.print("Distance: ");
        Serial.println(distance);
    }
}
