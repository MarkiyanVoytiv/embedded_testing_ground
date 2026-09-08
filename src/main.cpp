#include <Arduino.h>

#define RELAY_IN 4
#define RELAY_CONTROL_OUT 8

#define LOOP_DELAY_MS 1000
#define NUMBER_OF_TESTS 20

volatile bool onTest = false;
volatile unsigned long testFinishedTime = 0;

static void relayIsr()
{
    if (onTest && !testFinishedTime)
    {
        testFinishedTime = micros();
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(RELAY_IN, INPUT_PULLUP);
    pinMode(RELAY_CONTROL_OUT, OUTPUT);
    digitalWrite(RELAY_CONTROL_OUT, LOW);
    attachInterrupt(
        digitalPinToInterrupt(RELAY_IN),
        relayIsr,
        CHANGE);
    delay(5000);
}

void loop()
{
    static unsigned long testStartedTime = 0;
    static unsigned long testCount = 0;
    static unsigned long onSum = 0;
    static unsigned long offSum = 0;
    static unsigned long onCount = 0;
    static unsigned long offCount = 0;
    static bool relayOn = false;

    if (onTest && testFinishedTime > 0)
    {
        onTest = false;
        Serial.print("Test #");
        Serial.print(testCount + 1);
        Serial.println(relayOn ? " ON:" : " OFF:");
        Serial.print("Relay reaction time: ");
        unsigned long reactionTime = testFinishedTime - testStartedTime;
        Serial.println(reactionTime);
        if (relayOn)
        {
            onSum += reactionTime;
            onCount++;
        }
        else
        {
            offSum += reactionTime;
            offCount++;
        }
        testFinishedTime = 0;
        testCount++;
        delay(LOOP_DELAY_MS);
    }

    if (!onTest && testCount < NUMBER_OF_TESTS)
    {
        testStartedTime = micros();
        relayOn = !relayOn;
        onTest = true;
        relayOn ? digitalWrite(RELAY_CONTROL_OUT, HIGH) : digitalWrite(RELAY_CONTROL_OUT, LOW);
    }

    if (testCount == NUMBER_OF_TESTS)
    {
        Serial.println("Test finished.");
        Serial.print("Average ON reaction time: ");
        Serial.println(onCount ? onSum / onCount : 0);
        Serial.print("Average OFF reaction time: ");
        Serial.println(offCount ? offSum / offCount : 0);
        testCount++;
    }
}
