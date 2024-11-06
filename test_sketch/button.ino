#include <Arduino.h>

const int BUTTON_PIN = 2;

const int DELAY_TIME = 100;
const long NUM_BLINKS = 0;
bool on = false;

void setup() {
    // Initialize digital pin LED_BUILTIN as an output
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(115200);
}

void loop() {
    bool new_on = digitalRead(BUTTON_PIN) == HIGH;

    if (new_on != on) {
        digitalWrite(LED_BUILTIN, new_on);
    }

    on = new_on;

    Serial.println(">var1: " + String((int) on));

    delay(DELAY_TIME);

}
