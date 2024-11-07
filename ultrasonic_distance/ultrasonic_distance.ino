const int trigPin = 2;    // Trig pin
const int echoPin = 3;   // Echo pin

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
 
  // Original Echo/Trig method
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float echo_distance = duration * 0.034 / 2;
  
  // Print both readings
  Serial.println(">low:0,high:550,ecdist:" + String(echo_distance));
  delay(50);
}
