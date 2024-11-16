#include <MKS_SERVO42.h>

#define RX1 16
#define TX1 17

byte const stepper_id = 1;
uint8_t const speed = 1000;
uint16_t const pulses = 5000;

MKS_SERVO42_1_0 stepper;
HardwareSerial stepper_serial(1);

void setup() {
    Serial.begin(115200);
    stepper_serial.begin(57600, SERIAL_8N1, RX1, TX1);
    Serial.println("Serial1 started, initializing");
    stepper.initialize(&stepper_serial);
    Serial.println("Initialized");
    stepper.ping(stepper_id);
    Serial.println("Pinged");
}

byte direction = 0;
void loop() {
    direction = !direction;
    // long encoder_value = stepper.getCurrentPosition(stepper_id);
    // Serial.println("Encoder value: " + String(encoder_value));

    stepper.setTargetPosition(stepper_id, direction, speed, pulses);

    delay(100);
}
