#include <Wire.h>

bool resetDone = false;
byte oldTen = 0;

void setup() {

  Serial.begin(9600);  // start serial for output
  Wire.begin();        // join i2c bus (address optional for master)
 
}

void loop() {

  if(!resetDone){

    String command = "reset";
    
    for(int i=1;i<=9;i++){
      sendMessage(i,command);

      Wire.requestFrom(i, 8);
      String received = "";
      while (Wire.available()) { 
        char c = Wire.read();
        received += c; 
        
      }
      Serial.println((String)"received:" + received);
  
      if(received == "reset ok"){
        resetDone = true;
      }
    }//end for

    
  }else{
    Serial.println("animating...");
    animate();
  }

  delay(500);
}

void animate(){
  int ten = tenPart();
  if(ten != oldTen){

    String command = "";
    
    oldTen = ten;
    switch(oldTen){
      case 0:
        command = "0,0,5,0";
        break;
      case 1:
        command = "45,45,5,0";
        break;
      case 2:
        command = "90,90,5,0";
        break;
      case 3:
        command = "135,135,5,0";
        break;
      case 4:
        command = "180,180,5,0";
        break;
      case 5:
        command = "225,225,5,0";
        break;
        
    }

    Serial.println("sending "+command);
    for(int i=1;i<=9;i++){
      sendMessage(i,command);
      Wire.requestFrom(i, 8);
    }

  }
}

void sendMessage(int destination, String message){
    Wire.beginTransmission(destination);
    Wire.write(message.c_str());
    Wire.endTransmission();
}

int getSeconds(){
  unsigned long ms = millis();
  return (ms/1000)%60;
}

int tenPart(){
  return getSeconds()/10;
}
