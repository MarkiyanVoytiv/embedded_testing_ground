#include <Arduino.h>

volatile uint32_t interruptCount = 0;

struct Config
{
    static constexpr uint8_t BUTTON_PIN = 16;
};

void IRAM_ATTR handleButtonPress()
{
    interruptCount++;
}

void setup()
{
    Serial.begin(115200);

    pinMode(Config::BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(Config::BUTTON_PIN),
        handleButtonPress,
        FALLING);
}

void loop()
{
    static uint32_t prevInterruptCount = 0;
    if (interruptCount != prevInterruptCount)
    {
        Serial.println(interruptCount);
        prevInterruptCount = interruptCount;
    }
}