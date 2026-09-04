#include <Arduino.h>

struct Config
{
    static constexpr uint8_t GREEN_LED_PIN = 12;
    static constexpr uint8_t BLUE_LED_PIN = 8;
    static constexpr uint8_t RED_LED_PIN = 6;

    static constexpr unsigned long GREEN_BLINK_DELAY_MS = 200;
    static constexpr unsigned long BLUE_BLINK_DELAY_MS = 500;
    static constexpr unsigned long RED_BLINK_DELAY_MS = 1000;
};

void setup()
{
    pinMode(Config::GREEN_LED_PIN, OUTPUT);
    pinMode(Config::BLUE_LED_PIN, OUTPUT);
    pinMode(Config::RED_LED_PIN, OUTPUT);

    digitalWrite(Config::GREEN_LED_PIN, LOW);
    digitalWrite(Config::BLUE_LED_PIN, LOW);
    digitalWrite(Config::RED_LED_PIN, LOW);
}

void loop()
{
    unsigned long currentTime = millis();
    static unsigned long greenBlinkStartTime = currentTime;
    static unsigned long blueBlinkStartTime = currentTime;
    static unsigned long redBlinkStartTime = currentTime;

    if (currentTime - greenBlinkStartTime >= Config::GREEN_BLINK_DELAY_MS)
    {
        digitalWrite(Config::GREEN_LED_PIN, !digitalRead(Config::GREEN_LED_PIN));
        greenBlinkStartTime = currentTime;
    }

    if (currentTime - blueBlinkStartTime >= Config::BLUE_BLINK_DELAY_MS)
    {
        digitalWrite(Config::BLUE_LED_PIN, !digitalRead(Config::BLUE_LED_PIN));
        blueBlinkStartTime = currentTime;
    }

    if (currentTime - redBlinkStartTime >= Config::RED_BLINK_DELAY_MS)
    {
        digitalWrite(Config::RED_LED_PIN, !digitalRead(Config::RED_LED_PIN));
        redBlinkStartTime = currentTime;
    }
}