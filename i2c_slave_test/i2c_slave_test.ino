#include <Wire.h>
#include <Clock.h>


byte id = 1;                        //id de l'horloge
Clock clock = Clock(A1,2,3,A2,4,5); //parametrage de l'horloge (pin heure hall, pin heure step, pin heure direction, pin minute hall, pin minute step, pin minute direction)

String msg = "";                    //message temporaire i2c
byte state = 0;                     //etat de l'horloge
/*
 * 0 : check for calibrating
 * 1 : calibrating
 * 2 : calibrated
 */


/**
 * WORKFLOW :
 * Calibrage au demarrage
 * Execute les commandes quand calibrée
 */


void setup() {
  Serial.begin(9600);
  Wire.begin(id);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {

  switch(state){
    case 0:
      Serial.println("checking calibration...");
      if(clock.calibrated()){
        state = 2;
      }else{
        state = 1;
      }
      break;
    case 1:
    Serial.println("calibration...");
      clock.calibrate();
      state = 0;
      break;
    case 2:
    Serial.println("calibrated so animate");
      clock.animate();
      break;  
  }
  delay(10);
}

/**
 * Reception d'un message
 */
void receiveEvent(int howMany){
    msg = "";
    while (Wire.available()) { 
      char c = Wire.read();
      msg += c; 
    }
}

/**
 * Demande de reponse au precedent message
 * Si le message est CALIBRATED : retourne YES ou NOT
 * Sinon, on estime que c'est une commande de deplacement
 */
void requestEvent() {

  Serial.println("received:"+msg);
  String ans = "";

  if(msg=="CALIBRATED"){

    ans= clock.calibrated() ? "YES" : "NOT";
    Serial.println("responding:"+ans);
    Wire.write(ans.c_str());
  }else if(msg != ""){

    String parameters[4];
    int StringCount = 0;

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
    Wire.write("RUN");
  }
}
