#include <LiquidCrystal_I2C.h>


const int CLK_PIN = 2;
const int DT_PIN = 3;
const int SW_PIN = 4;

int numResets = 0;
const int debounceDelay = 2;

LiquidCrystal_I2C lcd(
    0x27, // I2C address
    16,
    2
);

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

    int offset() {
        return _offset;
    }

    void reset() {
        _offset = 0;
    }
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


void setup() {
  Serial.begin(115200);

  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Rotary: 0");
  lcd.setCursor(0, 1);
  lcd.print("Resets: 0");
}

void printState() {
  lcd.setCursor(8, 0);
  lcd.print("       ");
  lcd.setCursor(8, 0);
  lcd.print(String(rotary.offset()));

  lcd.setCursor(8, 1);
  lcd.print("       ");
  lcd.setCursor(8, 1);
  lcd.print(String(numResets));
}

void loop() {
  int rotation = rotary.tick();

  bool shouldPrintState = false;

  if (rotation != 0) {
    shouldPrintState = true;
  }

  int buttonPressed = rotaryButton.getState();

  if (buttonPressed != lastButtonState && buttonPressed == LOW) {
    shouldPrintState = true;
    lastButtonState = buttonPressed;
    numResets++;
    rotary.reset();
  }
  lastButtonState = buttonPressed;

  if (shouldPrintState) {
    printState();
  }

  if (millis() % 50 == 0) {
    Serial.println(">rotaryCLK:" + String(rotary.lastStateCLK) + ",rotaryDT:" + String(rotary.lastStateDT)+",rotarySW:" + String(buttonPressed));
  }
}
