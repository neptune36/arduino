#include "Arduino.h"
#include "ClockBluetooth.h"

void (*ClockBluetooth::user_onReceive)(String);

ClockBluetooth::ClockBluetooth(uint8_t rxPin,uint8_t txPin){
	pinMode(rxPin,INPUT);
	pinMode(txPin,OUTPUT);
	hc05 = new SoftwareSerial(rxPin ,txPin);
	hc05->begin(9600);
}

void ClockBluetooth::loop(){
	readBluetooth();
}
	

void ClockBluetooth::readBluetooth(){
	
	String bluetoothMessage = "";
	
  while (hc05->available()) {
    delay(10); 
    if (hc05->available() >0) {
      char c = hc05->read();  //gets one byte from serial buffer
      bluetoothMessage += c; //makes the string readString
    }
  }
  hc05->flush();

  if(bluetoothMessage!=""){
    user_onReceive(bluetoothMessage);
  }
 }

void ClockBluetooth::sendMessage(String message){
  Serial.print("Message bluetooth envoyé:");
  Serial.println(message);
  hc05->println(message);
}

void ClockBluetooth::onReceive(void (*function)(String)){
	user_onReceive = function;
}