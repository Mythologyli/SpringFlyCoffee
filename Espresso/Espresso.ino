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
    Serial.println("hello, latte!");
    delay(100);
}
