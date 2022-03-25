#include <Arduino.h>

#include "pump.h"

#define SERIAL_BAUD 115200

Pump pump1(2, 3, 4);
Pump pump2(5, 6, 7);

void setup()
{
    pump1.init();
    pump2.init();

    Serial.begin(SERIAL_BAUD);
}

void loop()
{
    switch (Serial.read())
    {
    case '1':
        pump1.start();
        break;

    case '2':
        pump2.start();
        break;

    case '3':
        pump1.stop();
        break;

    case '4':
        pump2.stop();
        break;

    default:
        break;
    }
}
