#include <Wire.h>
#include <DS3231.h>
#include <SoftwareSerial.h>
#define rxPin 2 //connecté sur tx du module
#define txPin 3 //connecté sur rx du module

DS3231 Clock;
byte Year ;
byte Month ;
byte Date ;
byte DoW ;
byte Hour ;
byte Minute ;
byte Second ;
bool Century  = false;
bool h12 ;
bool PM ;

String msg;

SoftwareSerial hc05(rxPin ,txPin);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 Serial.println(F("Initialize System"));
 Wire.begin();

 pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);

  hc05.begin(9600);
}

void loop() {
 //setDate();//Comment once the date is set
 //readRTC();

 readHCPort();
 processResponse();
   
}

/*
 * Retourne la date et heure du module RTC au format yyyy-MM-dd HH:mm:ss
 */
String readRTC( ) { 

  String dateFormatted = "20";
  
  dateFormatted = dateFormatted+Clock.getYear()+"-"+
  formatDateHour(Clock.getMonth(Century))+"-"+
  formatDateHour(Clock.getDate())+" "+
  formatDateHour(Clock.getHour(h12, PM))+":"+
  formatDateHour(Clock.getMinute())+":"+
  formatDateHour(Clock.getSecond())
  ;
 
  Serial.println(dateFormatted);

  return dateFormatted;
}

/*
 * Ajoute un zero devant la valeur si inferieur a 10 (ex: 9 -> 09)
 */
String formatDateHour(byte toFormat){

   int intValue = int(toFormat);
   String formatted = "";
   String decChar = intValue <10 ? "0" : "";
 
  formatted = decChar + intValue;

  return formatted;
  
}

void setDate(String data) { /* function setDate */
 ////Set Real Time Clock
   //int _start = millis();
    char buffer[data.length() + 1];
    data.toCharArray(buffer, data.length() + 1);
   GetDateStuff(buffer,Year, Month, Date, DoW, Hour, Minute, Second);
   Clock.setClockMode(false);  // set to 24h
   Clock.setSecond(Second);
   Clock.setMinute(Minute);
   Clock.setHour(Hour);
   Clock.setDate(Date);
   Clock.setMonth(Month);
   Clock.setYear(Year);
   Clock.setDoW(DoW);
 
}
void GetDateStuff(char InString[], byte& Year, byte& Month, byte& Day, byte& DoW, byte& Hour, byte& Minute, byte& Second) { /* function GetDateStuff */
 ////Get date data
 // Call this if you notice something coming in on
 // the serial port. The stuff coming in should be in
 // the order YYMMDDwHHMMSS, with an 'x' at the end.
 byte Temp1, Temp2;
 // Read Year first
 Temp1 = (byte)InString[0] - 48;
 Temp2 = (byte)InString[1] - 48;
 Year = Temp1 * 10 + Temp2;
 // now month
 Temp1 = (byte)InString[2] - 48;
 Temp2 = (byte)InString[3] - 48;
 Month = Temp1 * 10 + Temp2;
 // now date
 Temp1 = (byte)InString[4] - 48;
 Temp2 = (byte)InString[5] - 48;
 Day = Temp1 * 10 + Temp2;
 // now Day of Week
 DoW = (byte)InString[6] - 48;
 // now Hour
 Temp1 = (byte)InString[7] - 48;
 Temp2 = (byte)InString[8] - 48;
 Hour = Temp1 * 10 + Temp2;
 // now Minute
 Temp1 = (byte)InString[9] - 48;
 Temp2 = (byte)InString[10] - 48;
 Minute = Temp1 * 10 + Temp2;
 // now Second
 Temp1 = (byte)InString[11] - 48;
 Temp2 = (byte)InString[12] - 48;
 Second = Temp1 * 10 + Temp2;
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

void processResponse(){
  
   if(msg!=""){
    Serial.print("Message received from bluetooth: "+msg);
      
    String response = "";

    String command = msg.substring(0,3);

    if(command=="CST"){// SET TIME (eg: CST:210912w162200)
      setDate(msg.substring(4,17));
      response = "time is now set on:"+readRTC();
    }
    else if(command=="CGT"){// GET TIME
      response = readRTC();
    }
    else{
      response = msg + "=unknown command";
    }

    Serial.println("response:"+response);
    hc05.println(response);
      
    msg=""; 
  }
}
