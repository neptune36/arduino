void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead (A1)==HIGH){
     Serial.println("HIG");
    }else{
     Serial.println("LOW");
    }
}
