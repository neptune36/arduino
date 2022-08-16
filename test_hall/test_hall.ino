const byte digital_sensorPin = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode (digital_sensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  bool sensor_Val = digitalRead (digital_sensorPin);
  //
  /*if(sensor_Val == HIGH)
  {
    Serial.println("high");
    //digitalWrite (ledPin, HIGH);
  }
  if(sensor_Val == LOW)
  {
    Serial.println("low");
    //digitalWrite (ledPin, LOW);
  }*/
  Serial.println (sensor_Val, DEC);
  delay(300);
}
