#include <Arduino.h>

#include "pump.h"
#include "hcsr04.h"

#define SERIAL_BAUD 115200

Pump pump1(2, 3);
Pump pump2(4, 5);
HcSr04 hcsr04(6, 7);

void setup()
{
    pump1.init();
    pump2.init();

    hcsr04.init();

    Serial.begin(SERIAL_BAUD);
}

void loop()
{
    char ch = Serial.read();
    Pump *pump = nullptr;

    if (ch == '1')
    {
        pump = &pump1;
    }
    else if (ch == '2')
    {
        pump = &pump2;
    }
    else
    {
        return;
    }

    unsigned long startTime = 0;
    unsigned long oldTotalOpenedTime = 0;
    unsigned long totalOpenedTime = 0;
    bool isOpened = false;

    while (true)
    {
        if (isOpened)
        {
            if (hcsr04.getDistance() < 4.0)
            {

                totalOpenedTime = millis() - startTime + oldTotalOpenedTime;
            }
            else
            {
                Serial.println('2');
                pump->stop();
                isOpened = false;
            }
        }
        else if (hcsr04.getDistance() < 4.0)
        {
            Serial.println('1');
            pump->start();
            isOpened = true;
            startTime = millis();
            oldTotalOpenedTime = totalOpenedTime;
        }

        ch = Serial.read();
        if (ch == '0' || totalOpenedTime >= 10000)
        {
            pump->stop();
            break;
        }
    }

    Serial.println('0');
}
