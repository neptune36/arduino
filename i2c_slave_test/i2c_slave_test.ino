#include <Wire.h>
#include <Clock.h>

byte id = 1; 
Clock clock(10,11);
bool calibrated = false;
String msg = "";
byte state = 0;
/*
 * 0 : nothing
 * 1 : calibrating
 * 2 : calibrated
 * 3 : animated
 */
String parameters[4];

void setup() {
  Serial.begin(9600);
  Wire.begin(id);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  clock.hourMotor = new ClockMotor(2,4,3,5);
  clock.minuteMotor = new ClockMotor(6,8,7,9);
  
}

void loop() {
  

  switch(state){
    case 1:
      clock.calibrate();
      if(clock.calibrated()){
        state = 2;
      }
      break;
    case 3:
      clock.animate();
      break;
  }

  delay(10);
}

void receiveEvent(int howMany){
    msg = "";
    while (Wire.available()) { 
      char c = Wire.read();
      msg += c; 
    }
}

void requestEvent() {

  Serial.println("received:"+msg);
  String ans = "";

  if(msg=="reset"){
    switch(state){
      case 0:
        state=1;
        ans="reseting";
        break;
      case 1:
        ans="reseting";
        break;
      case 2:
        ans="reset ok";
        break;
    }

    Serial.println("responding:"+ans);
    Wire.write(ans.c_str());
  }else if(msg != ""){

    int StringCount = 0;
    state = 3;
    while (msg.length() > 0)
    {
      int index = msg.indexOf(',');
      if (index == -1) // No comma found
      {
        parameters[StringCount++] = msg;
        Serial.println("param:"+msg);
        break;
      }
      else
      {
        parameters[StringCount++] = msg.substring(0, index);
        msg = msg.substring(index+1);
      }
    }

     for (int i = 0; i < StringCount; i++)
    {
      Serial.print(i);
      Serial.print(": \"");
      Serial.print(parameters[i]);
      Serial.println("\"");
    }

    clock.setDestination(parameters[0].toInt(),parameters[1].toInt(),parameters[2].toInt(),parameters[3].toInt());
    
    
  }
}
