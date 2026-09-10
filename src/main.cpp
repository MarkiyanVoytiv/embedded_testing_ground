#include <Arduino.h>

struct Config
{
    static constexpr uint8_t BUTTON_PIN = 16;
    static constexpr uint32_t POLL_INTERVAL_MS = 10;
    static constexpr uint8_t STABLE_READS_REQUIRED = 3;
};

enum class ButtonState
{
    Released,
    DebouncingPress,
    Pressed,
    DebouncingRelease
};

void setup()
{
    Serial.begin(115200);

    pinMode(Config::BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
    static ButtonState buttonState = ButtonState::Released;
    static uint32_t buttonPressCount = 0;
    static uint32_t lastPollTime = 0;
    static uint8_t stableReadCount = 0;

    uint32_t currentTime = millis();

    if (currentTime - lastPollTime < Config::POLL_INTERVAL_MS)
    {
        return;
    }

    lastPollTime = currentTime;

    bool buttonPressed = digitalRead(Config::BUTTON_PIN) == LOW;

    switch (buttonState)
    {
        case ButtonState::Released:
            if (buttonPressed)
            {
                stableReadCount = 1;
                buttonState = ButtonState::DebouncingPress;
            }
            break;

        case ButtonState::DebouncingPress:
            if (!buttonPressed)
            {
                stableReadCount = 0;
                buttonState = ButtonState::Released;
            }
            else if (++stableReadCount >= Config::STABLE_READS_REQUIRED)
            {
                stableReadCount = 0;
                buttonState = ButtonState::Pressed;

                buttonPressCount++;
                Serial.println(buttonPressCount);
            }
            break;

        case ButtonState::Pressed:
            if (!buttonPressed)
            {
                stableReadCount = 1;
                buttonState = ButtonState::DebouncingRelease;
            }
            break;

        case ButtonState::DebouncingRelease:
            if (buttonPressed)
            {
                stableReadCount = 0;
                buttonState = ButtonState::Pressed;
            }
            else if (++stableReadCount >= Config::STABLE_READS_REQUIRED)
            {
                stableReadCount = 0;
                buttonState = ButtonState::Released;
            }
            break;
    }
}