#include <Arduino.h>

enum class TrafficState
{
    MovementAllowed,
    MovementEnding,
    ClearIntersection,
    Stop,
    PrepareToMove
};

enum class LedMode
{
    Off,
    On,
    Blinking
};

struct Config
{
    static constexpr uint8_t GREEN_LED_PIN = 9;
    static constexpr uint8_t YELLOW_LED_PIN = 8;
    static constexpr uint8_t RED_LED_PIN = 16;

    static constexpr unsigned long MOVEMENT_ALLOWED_TIME_MS = 5000;
    static constexpr unsigned long MOVEMENT_ENDING_TIME_MS = 3000;
    static constexpr unsigned long CLEAR_INTERSECTION_TIME_MS = 2000;
    static constexpr unsigned long STOP_TIME_MS = 5000;
    static constexpr unsigned long PREPARE_TO_MOVE_TIME_MS = 2000;

    static constexpr unsigned long BLINK_DELAY_MS = 500;
};

unsigned long getStateDurationMs(TrafficState state)
{
    switch (state)
    {
    case TrafficState::MovementAllowed:
        return Config::MOVEMENT_ALLOWED_TIME_MS;

    case TrafficState::MovementEnding:
        return Config::MOVEMENT_ENDING_TIME_MS;

    case TrafficState::ClearIntersection:
        return Config::CLEAR_INTERSECTION_TIME_MS;

    case TrafficState::Stop:
        return Config::STOP_TIME_MS;

    case TrafficState::PrepareToMove:
        return Config::PREPARE_TO_MOVE_TIME_MS;
    }

    return 0;
}

class Led
{
public:
    Led(uint8_t pin) : pin(pin)
    {
    }

    void init()
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    void set(LedMode newMode)
    {
        mode = newMode;

        if (mode == LedMode::On)
        {
            digitalWrite(pin, HIGH);
        }
        else if (mode == LedMode::Off)
        {
            digitalWrite(pin, LOW);
        }
        else if (mode == LedMode::Blinking)
        {
            lastBlinkTime = millis();
        }
    }

    void update()
    {
        if (mode != LedMode::Blinking)
        {
            return;
        }

        unsigned long currentTime = millis();

        if (currentTime - lastBlinkTime >= Config::BLINK_DELAY_MS)
        {
            lastBlinkTime = currentTime;
            digitalWrite(pin, !digitalRead(pin));
        }
    }

private:
    uint8_t pin;
    LedMode mode = LedMode::Off;
    unsigned long lastBlinkTime = 0;
};

Led greenLed(Config::GREEN_LED_PIN);
Led yellowLed(Config::YELLOW_LED_PIN);
Led redLed(Config::RED_LED_PIN);

TrafficState trafficState = TrafficState::MovementAllowed;
unsigned long stateStartTime = 0;

void setup()
{
    Serial.begin(115200);

    greenLed.init();
    yellowLed.init();
    redLed.init();

    greenLed.set(LedMode::On);

    stateStartTime = millis();
}

void loop()
{
    greenLed.update();
    yellowLed.update();
    redLed.update();

    unsigned long currentTime = millis();

    if (currentTime - stateStartTime < getStateDurationMs(trafficState))
    {
        return;
    }

    stateStartTime = currentTime;

    switch (trafficState)
    {
    case TrafficState::MovementAllowed:
        trafficState = TrafficState::MovementEnding;

        greenLed.set(LedMode::Blinking);
        Serial.println("MovementEnding");
        break;

    case TrafficState::MovementEnding:
        trafficState = TrafficState::ClearIntersection;

        greenLed.set(LedMode::Off);
        yellowLed.set(LedMode::On);
        Serial.println("ClearIntersection");
        break;

    case TrafficState::ClearIntersection:
        trafficState = TrafficState::Stop;

        yellowLed.set(LedMode::Off);
        redLed.set(LedMode::On);
        Serial.println("Stop");
        break;

    case TrafficState::Stop:
        trafficState = TrafficState::PrepareToMove;

        redLed.set(LedMode::On);
        yellowLed.set(LedMode::On);
        Serial.println("PrepareToMove");
        break;

    case TrafficState::PrepareToMove:
        trafficState = TrafficState::MovementAllowed;

        redLed.set(LedMode::Off);
        yellowLed.set(LedMode::Off);
        greenLed.set(LedMode::On);
        Serial.println("MovementAllowed");
        break;
    }
}