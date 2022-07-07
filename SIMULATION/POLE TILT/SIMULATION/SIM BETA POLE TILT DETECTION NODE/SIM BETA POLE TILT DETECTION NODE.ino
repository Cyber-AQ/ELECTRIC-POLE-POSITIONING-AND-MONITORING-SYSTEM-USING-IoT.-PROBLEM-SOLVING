int tilt_sensor = A0, tilt_limit = 920;

void setup() {
  Serial.begin(9600);
  pinMode(tilt_sensor, INPUT);

}

void loop() {
  int sensor_input = analogRead(tilt_sensor);
  if(sensor_input < tilt_limit) Serial.println("NODE A: too much tilt!");
  else Serial.println("NODE B: pole safe");
  delay(1000);
}
