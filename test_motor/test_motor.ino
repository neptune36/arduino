#include <ClockMotor.h>

ClockMotor m = ClockMotor(2,4,3,5);
byte oldTen = -1;
int memAngle = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  m.logging = true;

}

void loop() {

  int ten = tenPart();
  
  if(ten != oldTen){
    oldTen = ten;

    //testC();
    //testB();
  testA();
    
      
    }

  m.rotate();

delay(2);
  
}

void testC(){

  memAngle+=45;
  memAngle = memAngle % 360;
  m.setDestination(memAngle,5,0);
  
}

void testB(){

int angle = 0;

  if(oldTen%2==0){
    angle = 180;
  }else{
    angle = 0;
  }
  Serial.println((String)"angle:" +angle);
  m.setDestination(angle,5,0);
}

void testA(){
  
  int angle = 0;
    int secondsToReachAngle = 5;
    int rotationMode = 0; //(0=le sens le plus pret pour atteindre la destination,1=sens horaire,2=sens anti-horaire)
  //en fonction de cette valeur, on joue une animation differente
    switch(oldTen){
      case 0:
        angle = 270;
        break;
       case 1:
        angle = 180;
        break;
       case 2:
        angle = 135;
        break;
       case 3:
        angle = 90;
        break;
       case 4:
        angle = 45;
        break;
       case 5:
        angle = 0;
        break;
    }
      m.setDestination(angle,secondsToReachAngle,rotationMode);
}



int getSeconds(){
  unsigned long ms = millis();
  return (ms/1000)%60;
}

int tenPart(){
  return getSeconds()/10;
}

int degreesToSteps(int degree){
  return (float)2048 / 360 * degree;
}
