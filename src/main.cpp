#include <Arduino.h>

#define LED_RED_PIN 15
#define LED_BLUE_PIN 18
#define LED_UNI_PIN 5
#define SPEED_BUTTON_PIN 10
#define MODE_BUTTON_PIN 12

bool ledState = false;

bool fastMode = false;
bool prevSpeedButtonState = HIGH;

bool patrolMode = false;
bool prevModeButtonState = HIGH;

unsigned long blinkDelay = 1000;

void setup() {
    Serial.begin(115200);

    pinMode(LED_BLUE_PIN, OUTPUT);
    digitalWrite(LED_BLUE_PIN, LOW);

    pinMode(LED_RED_PIN, OUTPUT);
    digitalWrite(LED_RED_PIN, LOW);

    pinMode(LED_UNI_PIN, OUTPUT);
    digitalWrite(LED_UNI_PIN, LOW);

    pinMode(SPEED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);

}

void loop() {
  bool speedButtonState = digitalRead(SPEED_BUTTON_PIN);
  if (prevSpeedButtonState == HIGH && speedButtonState == LOW) {
    delay(30);
        fastMode = !fastMode;
        if (fastMode)
        {
          blinkDelay = 200;
          Serial.println("Speed Mode: FAST");
        }
        else
        {
          blinkDelay = 1000;
          Serial.println("Speed Mode: SLOW");
        }


    }
    prevSpeedButtonState = speedButtonState;

  bool modeButtonState = digitalRead(MODE_BUTTON_PIN);
    if (prevModeButtonState == HIGH && modeButtonState == LOW){
      delay(30);
        patrolMode = !patrolMode;

        if (patrolMode){
          Serial.println("Mode: PATROL");
        }else{
          Serial.println("Mode: NORMAL");
        }

    }
    prevModeButtonState = modeButtonState;

    static unsigned long previousTime = 0;
    if (millis() - previousTime >= blinkDelay)
    {
        previousTime = millis();
        if (ledState) {
          digitalWrite(LED_BLUE_PIN, LOW);
          if (patrolMode) {
            pinMode(LED_UNI_PIN, INPUT);
          } else {
            pinMode(LED_UNI_PIN, OUTPUT);
            digitalWrite(LED_UNI_PIN, LOW);
            digitalWrite(LED_RED_PIN, HIGH);
          }
        
          ledState = false;
          Serial.println("LED_BLUE OFF");
    } else {
          digitalWrite(LED_BLUE_PIN, HIGH);
          digitalWrite(LED_RED_PIN, LOW);

          pinMode(LED_UNI_PIN, OUTPUT);
          digitalWrite(LED_UNI_PIN, HIGH);

          ledState = true;
          Serial.println("LED_BLUE ON");
    }
    }
    
}