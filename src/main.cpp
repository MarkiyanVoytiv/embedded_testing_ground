#include <Arduino.h>

#define LED_RED_PIN 15
#define LED_BLUE_PIN 18

#define SYNC_BUTTON_PIN 10
#define ASYNC_BUTTON_PIN 0

void setup()
{
    Serial.begin(115200);

    pinMode(LED_BLUE_PIN, OUTPUT);
    digitalWrite(LED_BLUE_PIN, LOW);

    pinMode(LED_RED_PIN, OUTPUT);
    digitalWrite(LED_RED_PIN, LOW);

    pinMode(SYNC_BUTTON_PIN, INPUT_PULLUP);
    pinMode(ASYNC_BUTTON_PIN, INPUT);
}

void loop()
{
    static bool syncMode = true;   // режим №1 за замовчуванням
    static bool ledState = false;

    static unsigned long previousTime = 0;

    unsigned long blinkDelay;

    bool syncButtonState = digitalRead(SYNC_BUTTON_PIN);
    bool asyncButtonState = digitalRead(ASYNC_BUTTON_PIN);

    if (syncButtonState == LOW && !syncMode)
    {
        delay(30);
        syncMode = true;
        Serial.println("Sync Mode Activated");
    }

    if (asyncButtonState == LOW && syncMode)
    {
        delay(30);
        syncMode = false;
        Serial.println("Async Mode Activated");
    }

    if (syncMode)
    {
        blinkDelay = 200;
    }
    else
    {
        blinkDelay = 1000;
    }

    if (millis() - previousTime >= blinkDelay)
    {
        previousTime = millis();
        ledState = !ledState;

        if (syncMode)
        {
            digitalWrite(LED_RED_PIN, ledState);
            digitalWrite(LED_BLUE_PIN, ledState);
        }
        else
        {
            digitalWrite(LED_RED_PIN, ledState);
            digitalWrite(LED_BLUE_PIN, !ledState);
        }
    }
}