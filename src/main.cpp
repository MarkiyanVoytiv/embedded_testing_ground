#include <Arduino.h>
#define RELAY_CONTROL_OUT 8
#define ON_TIME_US 2000000
#define OFF_TIME_US 6000000

hw_timer_t *timer = NULL;

volatile bool stateChanged = false;
volatile bool relayState = false;

void IRAM_ATTR onTimer()
{
    relayState = !relayState;
    digitalWrite(RELAY_CONTROL_OUT, relayState);
    timerAlarmWrite(timer, relayState ? ON_TIME_US : OFF_TIME_US, true);
    stateChanged = true;
}

void setup()
{
    Serial.begin(115200);

    pinMode(RELAY_CONTROL_OUT, OUTPUT);
    digitalWrite(RELAY_CONTROL_OUT, LOW);

    // Initialize timer 0 with prescaler 80, counting up
    timer = timerBegin(0, 80, true);

    // Attach interrupt handler
    timerAttachInterrupt(timer, &onTimer, true);

    // Set initial alarm interval
    timerAlarmWrite(timer, OFF_TIME_US, true);

    // Enable timer alarm
    timerAlarmEnable(timer);
}

void loop()
{
    if (stateChanged) {
        stateChanged = false;
        Serial.print("Relay State: ");
        Serial.println(relayState ? "ON" : "OFF");
    }
}