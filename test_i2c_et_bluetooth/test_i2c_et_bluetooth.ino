//bluetooth params
#include "ClockBluetooth.h"

ClockBluetooth bluetooth(2 ,3);

//I2C params
#include <Wire.h>


void setup() {
  Serial.begin(9600);
  bluetooth.onReceive(proceedBluetooth);

  //I2C setup
  Wire.begin();
}

void loop() {

  bluetooth.loop();

}

void proceedBluetooth(String message){
  Serial.println(message);
  if(message == "test"){
    bluetooth.sendMessage("test ok");
  }else{//exemple "225,225,5,0";
    writeI2C(1,message);
    readI2C(1,8);
  }
}

// I2C FUNCTIONS
void readI2C(int from, int expectedLength){
  Wire.requestFrom(from, expectedLength);
  String received = "";
  while (Wire.available()) { 
    char c = Wire.read();
    received += c;    
  }
  Serial.println((String)"Message I2C recu de [" + from + "]:" + received);
}

void writeI2C(int destination, String message){
  Wire.beginTransmission(destination);
  Wire.write(message.c_str());
  Wire.endTransmission();
  Serial.println((String)"Message I2C envoyé à [" + destination + "]:" + message);
}
