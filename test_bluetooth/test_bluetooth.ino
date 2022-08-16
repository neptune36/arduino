#include <SoftwareSerial.h>
#define rxPin 2 //connecté sur tx du module
#define txPin 3 //connecté sur rx du module
#define baudrate 9600

/*
AT : Ceck the connection.
AT+NAME : See default name
AT+ADDR : see default address
AT+VERSION : See version
AT+UART : See baudrate
AT+ROLE: See role of bt module(1=master/0=slave)
AT+RESET : Reset and exit AT mode
AT+ORGL : Restore factory settings
AT+PSWD: see default password
AT+NAME=NINE-O-CLOCK
 */

String msg;

SoftwareSerial hc05(rxPin ,txPin);

void setup(){
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");
  hc05.begin(baudrate);
}

void loop(){
    //readSerialPort();
    readHCPort();
    if(msg!=""){
      Serial.print("Master sent : " );
      Serial.println(msg);
      hc05.println(msg);
      msg=""; 
 }
}

void readSerialPort(){
  msg="";
 while (Serial.available()) {
   delay(10);  
   if (Serial.available() >0) {
     char c = Serial.read();  //gets one byte from serial buffer
     msg += c; //makes the string readString
   }
 }
}

void readHCPort(){
while (hc05.available()) {
  delay(10); 
  if (hc05.available() >0) {
    char c = hc05.read();  //gets one byte from serial buffer
    msg += c; //makes the string readString
  }
}
hc05.flush();
}
