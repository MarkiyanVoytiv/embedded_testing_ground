#include <Arduino.h>

volatile bool interruptFlag = false;

struct Config
{
    static constexpr uint8_t BUTTON_PIN = 16;
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
    static uint32_t buttonPressCount = 0;
    static bool buttonPressed = false;

    bool buttonState = digitalRead(Config::BUTTON_PIN);

    if (interruptFlag)
    {
        interruptFlag = false;

        if (buttonState == LOW && !buttonPressed)
        {
            buttonPressCount++;
            buttonPressed = true;
            Serial.println(buttonPressCount);
        }
    }

    if (buttonState == HIGH && buttonPressed)
    {
        buttonPressed = false;
    }
}