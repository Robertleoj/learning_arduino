#include <ezButton.h>
#include <Arduino.h>

const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;
const int JOYSTICK_BUTTON_PIN = 3;

ezButton button(JOYSTICK_BUTTON_PIN);

void setup() {
    Serial.begin(115200);

    button.setDebounceTime(50);

    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);
}

void loop() {
    int x = analogRead(JOYSTICK_X_PIN);
    int y = analogRead(JOYSTICK_Y_PIN);
    bool button_pressed = button.isPressed();

    Serial.println(">x:" + String(x) + ",y:" + String(y) + ",button:" + String(button_pressed));

    delay(10);
}