#include "ClockBluetooth.h"
#include "ClockI2C.h"

ClockBluetooth bluetooth(2 ,3);
ClockI2C clockI2C;


bool calibrated = false;
byte oldTen = 0;

void setup() {
  Serial.begin(9600);
  bluetooth.onReceive(proceedBluetooth);
}

void loop() {

  //bluetooth.loop();

  if(!calibrated){

    calibrated = true;

    for(int i=1;i<=9;i++){
      String ans = clockI2C.askQuestionTo(i,"CALIBRATED",3);
      if(ans!="YES"){
        calibrated = false;
        break;
      }
    }
  }else{
    animate();
  }

  delay(200); //necessaire ?

}

//BLUETOOTH EVENT
void proceedBluetooth(String message){
  Serial.println(message);
  if(message == "test"){
    bluetooth.sendMessage("test ok");
  }else{//exemple "225,225,5,0";
    
  }
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

    
    for(int i=1;i<=9;i++){
      clockI2C.askQuestionTo(i,command,3);
    }

  }
}

int getSeconds(){
  unsigned long ms = millis();
  return (ms/1000)%60;
}

int tenPart(){
  return getSeconds()/10;
}
