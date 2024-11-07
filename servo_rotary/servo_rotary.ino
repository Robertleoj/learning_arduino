#include <Servo.h>

const int CLK_PIN = 2;
const int DT_PIN = 3;
const int SW_PIN = 4;

const int SERVO_PIN = 5;

int numResets = 0;
const int debounceDelay = 2;
const int rotaryRange = 10;

int lastButtonState = HIGH;

class Rotary {
   public:
    int lastStateCLK;
    int lastStateDT;
    int _offset;
    unsigned long lastDebounceTime;

    Rotary() {
        lastStateCLK = digitalRead(CLK_PIN);
        lastStateDT = digitalRead(DT_PIN);
        _offset = 0;
        lastDebounceTime = 0;
    }

    int tick() {
        int currentStateCLK = digitalRead(CLK_PIN);
        int currentStateDT = digitalRead(DT_PIN);
        int result = 0;

        // If enough time has passed since last change
        if ((millis() - lastDebounceTime) > debounceDelay) {
            // If CLK changed, it means rotation occurred
            if (currentStateCLK != lastStateCLK) {
                lastDebounceTime = millis();

                // Determine direction based on DT state
                if (currentStateDT != currentStateCLK) {
                    // Clockwise
                    result = 1;
                    _offset++;
                } else {
                    // Counter-clockwise
                    result = -1;
                    _offset--;
                }
            }
        }

        lastStateCLK = currentStateCLK;
        lastStateDT = currentStateDT;
        return result;
    }
    void constrainOffset(int min, int max) {
        if (_offset < min) _offset = min;
        if (_offset > max) _offset = max;
    }

    int offset() { return _offset; }

    void reset() { _offset = 0; }
};

class Button {
   private:
    const int _pin;
    int _lastState;
    int _currentState;
    unsigned long _lastDebounceTime;

   public:
    Button(int pin) : _pin(pin) {
        _lastState = HIGH;
        _currentState = HIGH;
        _lastDebounceTime = 0;
    }

    int getState() {
        int reading = digitalRead(_pin);

        // If the switch changed, due to noise or pressing
        if (reading != _lastState) {
            // Reset the debouncing timer
            _lastDebounceTime = millis();
        }

        // If enough time has passed since last change
        if ((millis() - _lastDebounceTime) > debounceDelay) {
            // If the button state has changed
            if (reading != _currentState) {
                _currentState = reading;
            }
        }

        _lastState = reading;
        return _currentState;
    }
};

Rotary rotary;
Button rotaryButton(SW_PIN);
Servo servo;

void setup() {
    Serial.begin(115200);
    servo.attach(SERVO_PIN);

    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);
}

void loop() {
    int rotation = rotary.tick();

    int buttonPressed = rotaryButton.getState();

    if (buttonPressed != lastButtonState && buttonPressed == LOW) {
        lastButtonState = buttonPressed;
        numResets++;
        rotary.reset();
    }
    lastButtonState = buttonPressed;

    rotary.constrainOffset(-rotaryRange, rotaryRange);
    int rotaryVal = rotary.offset();
    int servoVal = map(rotaryVal, -rotaryRange, rotaryRange, 1000, 2000);

    servo.writeMicroseconds(servoVal);

    if (millis() % 50 == 0) {
        Serial.println(">rotaryCLK:" + String(rotary.lastStateCLK) +
                       ",rotaryDT:" + String(rotary.lastStateDT) +
                       ",rotarySW:" + String(buttonPressed) +
                       ",servoVal:" + String(servoVal));
    }
}
