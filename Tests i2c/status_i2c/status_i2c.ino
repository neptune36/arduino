bool currentVal=LOW; // set initial value
uint8_t currentPin = A5;

void switchPin(){ // swap input and output pins
if(currentPin==A4){
  pinMode(A5,INPUT);
  pinMode(A4,OUTPUT);
  digitalWrite(A4,currentVal);
  currentPin=A5;
}
else {
  pinMode(A4,INPUT);
  pinMode(A5,OUTPUT);
  digitalWrite(A5,currentVal);
  currentPin=A4;
  }
}
void setup(){
Serial.begin(9600);
switchPin(); // setup initial input,output
Serial.println("booted");
}

void printState(){
if(currentPin==A4){
  Serial.print("A5 is outputting: ");
  Serial.println(currentVal,DEC);
  if(digitalRead(currentPin) != currentVal)  Serial.print("But, A4 is reading as: ");
  else Serial.print("A4 is correctly reading: ");
  Serial.println(digitalRead(currentPin),DEC);
  }
if(currentPin==A5){
  Serial.print("A4 is outputting: ");
  Serial.println(currentVal,DEC);
  if (digitalRead(currentPin) != currentVal)  Serial.print("But, A5 is reading as: ");
  else Serial.print("A5 is correctly reading: ");
  Serial.println(digitalRead(currentPin),DEC);
  }
}

void writeOther(uint8_t cPin,uint8_t cVal){
if(cPin==A4) digitalWrite(A5,cVal);
else digitalWrite(A4,cVal);
}

void loop(){
if(digitalRead(currentPin)==currentVal){
  currentVal = !currentVal;
  writeOther(currentPin,currentVal);
  }
else { // problem
  Serial.println(" INput does not match Output!");
  printState();
  return; // error out
  }

if(digitalRead(currentPin)==currentVal){
  switchPin();
  }
else { // problem
  Serial.println(" INput does not match Output!");
  printState();
  return; // error out
  }

printState();
delay(500); // 1/2 second between tests

}
