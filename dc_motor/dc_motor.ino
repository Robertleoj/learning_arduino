// const int enB = 3;
const int motor_b_pin1 = 4;
const int motor_b_pin2 = 5;

int forward = true;
const int delay_time = 1000;

void setup() {
    pinMode(motor_b_pin1, OUTPUT);
    pinMode(motor_b_pin2, OUTPUT);

}

void loop() {
    if (forward) {
        digitalWrite(motor_b_pin1, HIGH);
        digitalWrite(motor_b_pin2, LOW);
    } else {
        digitalWrite(motor_b_pin1, LOW);
        digitalWrite(motor_b_pin2, HIGH);
    }
    forward = !forward;

    delay(delay_time);
}
