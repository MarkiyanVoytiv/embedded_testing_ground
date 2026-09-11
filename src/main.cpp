#include <Arduino.h>
#define RELAY_CONTROL_OUT 8
#define ON_TIME_US 2000000
#define OFF_TIME_US 6000000

hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer()
{
    bool relayState = digitalRead(RELAY_CONTROL_OUT);
    digitalWrite(RELAY_CONTROL_OUT, !relayState);
    timerAlarmWrite(timer, relayState ? OFF_TIME_US : ON_TIME_US, true);
}

void setup()
{
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

}