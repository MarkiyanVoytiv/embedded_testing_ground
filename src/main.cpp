#include <Arduino.h>

volatile uint32_t interruptCount = 0;
volatile bool interruptFlag = false;

struct Config
{
    static constexpr uint8_t BUTTON_PIN = 16;
    static constexpr uint32_t DEBOUNCE_DELAY_MS = 50;
};

void IRAM_ATTR handleButtonPress()
{
    interruptFlag = true;
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
    static uint32_t lastAcceptedTime = 0;
    uint32_t currentTime = 0;
    if (interruptFlag)
    {
        interruptFlag = false;
        currentTime = millis();
        if (currentTime - lastAcceptedTime >= Config::DEBOUNCE_DELAY_MS)
        {
            interruptCount++;
            lastAcceptedTime = currentTime;
            Serial.println(interruptCount);
        }
    }
}