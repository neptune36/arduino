#include "Arduino.h"
#include "ClockI2C.h"

ClockI2C::ClockI2C(){
  Wire.begin();
  Wire.setWireTimeout();
}

String ClockI2C::askQuestionTo(int destination, String question, int responseLength){

  writeI2C(destination,question);
  return readI2C(destination,responseLength);
  
}

/**
 * expectedLength est la taille de la reponse attendue
 * la reponse sera tronquée si pas assez de bit, des ? seront rajoutés si trop de bit
 */
String ClockI2C::readI2C(int from, int expectedLength){
  Wire.requestFrom(from, expectedLength);
  String received = "";
  while (Wire.available()) { 
    char c = Wire.read();
    received += c;    
  }
  //Serial.println((String)"Message I2C recu de [" + from + "]:" + received);
  return received;
}

void ClockI2C::writeI2C(int destination, String message){
  Wire.beginTransmission(destination);
  Wire.write(message.c_str());
  Wire.endTransmission();
  //Serial.println((String)"Message I2C envoyé à [" + destination + "]:" + message);
}