#include <Arduino.h>

volatile bool buttonPressed = false;

struct Config
{
    static constexpr uint8_t LED_PIN = 4;
    static constexpr uint8_t BUTTON_PIN = 10;
    static constexpr unsigned long BLINK_DELAY_MS = 1000;
    static constexpr unsigned long DEBOUNCE_DELAY_MS = 200;
};

enum class LedState
{
    Off,
    On
};

enum class LedMode
{
    Blinking,
    On,
    Off
};

class Led
{
public:
    static void init()
    {
        pinMode(Config::LED_PIN, OUTPUT);
        set(LedState::Off);
        pinMode(Config::BUTTON_PIN, INPUT_PULLUP);
        attachInterrupt(
            digitalPinToInterrupt(Config::BUTTON_PIN),
            buttonIsr,
            FALLING);
    }

    static void buttonIsr()
    {
        buttonPressed = true;
    }

    static void set(LedState state)
    {
        if (state == LedState::On)
        {
            digitalWrite(Config::LED_PIN, HIGH);
        }
        else
        {
            digitalWrite(Config::LED_PIN, LOW);
        }
    }
};

void setup()
{
    Serial.begin(115200);
    Led::init();
}

void loop()
{
    unsigned long startTime = micros();
    static uint32_t totalLoopTime = 0;
    static unsigned long previousTime = 0;
    static LedState currentState = LedState::Off;
    static LedMode currentMode = LedMode::Blinking;
    static uint16_t loopCounter = 0;
    static unsigned long lastButtonPressTime = 0;

    if (buttonPressed)
    {
        buttonPressed = false;
        if (millis() - lastButtonPressTime > Config::DEBOUNCE_DELAY_MS)
        { 
            switch (currentMode)
            {
            case LedMode::Blinking:
                currentMode = LedMode::On;
                break;
            case LedMode::On:
                currentMode = LedMode::Off;
                break;
            case LedMode::Off:
                currentMode = LedMode::Blinking;
                break;
            }
            lastButtonPressTime = millis();
        }
    }

    if (currentMode == LedMode::On)
    {
        currentState = LedState::On;
        Led::set(currentState);
    }
    else if (currentMode == LedMode::Blinking)
    {
        if (millis() - previousTime >= Config::BLINK_DELAY_MS)
        {
            previousTime = millis();

            currentState =
                (currentState == LedState::Off)
                    ? LedState::On
                    : LedState::Off;

            Led::set(currentState);
        }
    }
    else if (currentMode == LedMode::Off)
    {
        currentState = LedState::Off;
        Led::set(currentState);
    }

    unsigned long endTime = micros();
    unsigned long loopDuration = endTime - startTime;

    totalLoopTime += loopDuration;
    loopCounter++;

    if (loopCounter == 1000)
    {
        Serial.println("Average loop time (microseconds):");
        Serial.println(totalLoopTime / loopCounter);
        loopCounter = 0;
        totalLoopTime = 0;
    }
}