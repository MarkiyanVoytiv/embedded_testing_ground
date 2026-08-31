#include <Arduino.h>

#define LDR_PIN 4

void setup()
{
    Serial.begin(115200);
    analogReadResolution(12);
}

void loop()
{
    int raw = analogRead(LDR_PIN);

    float calculatedVoltage =
        (raw / 4095.0) * 3100.0;

    uint32_t measuredVoltage =
        analogReadMilliVolts(LDR_PIN);

    float error =
        abs(calculatedVoltage - measuredVoltage)
        / measuredVoltage
        * 100.0;

    Serial.print("RAW: ");
    Serial.print(raw);

    Serial.print(" | Calculated: ");
    Serial.print(calculatedVoltage);
    Serial.print(" mV");

    Serial.print(" | Measured: ");
    Serial.print(measuredVoltage);
    Serial.print(" mV");

    Serial.print(" | Error: ");
    Serial.print(error, 2);
    Serial.println(" %");

    delay(100);
}