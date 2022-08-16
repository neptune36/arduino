//import de ma bibliotheque Clock
#include <Clock.h>

//definition de la variable clock(12:capteur heure,13:capteur minute)
Clock clock(12,13);
//variable memorisation horloge calibree
bool calibrated = false;
//variable contenant le chiffre des dizaines de secondes (0-9s=0,10-19s=1...)
byte oldTen = -1;

//methode appelé au demarrage du programme, initialisation du programme
void setup() {
  //affectation des moteurs à l'horloge
  clock.hourMotor = new ClockMotor(2,4,3,5);//moteur des heures sur les pins 2,3,4,5
  clock.minuteMotor = new ClockMotor(8,10,9,11);//moteur des minutes sur les pins 8,9,10,11

  /*le decalage sur l'ordre des ports moteur est normal dans le programme,
   * ca permet de les brancher dans l'ordre physiquement
   */
  
}

//boucle appelée toute les n µs
void loop() {

  //si l'horloge n'est pas calibrée (les 2 capteurs n'ont pas été declanchés)
  if(!calibrated){
    //on calibre l'horloge (les moteurs tournent indefiniment)
    clock.calibrate();
    /*on met a jour la variable de calibrage avec l'etat du calibrage de l'horloge :
     * il faut que les 2 capteurs aient été déclanchés pour que ca passe a TRUE
     * seul les moteurs qui n'on pas fait leur poitn zero tournent
     */
    calibrated = clock.calibrated();
    
  }else{
    //si l'horloge est calibrée, on joue les animations
    animate();
  }

  delay(2);//NOUVEAU :j'ai exclu le delai de la bib, je le met ici. Plus efficace, moins de retard dans les mouvements (ca devrait eviter les sauts)
  
}

//fonction de l'animation
void animate(){
  //on recupere la valeur des dizaines de seconde
  int ten = tenPart();

  //si elle est diferente de la derniere, on lance une animation (soit une nouvelle animation toutes les 10s)
  if(ten != oldTen){
    oldTen = ten;//on met a jour la variable avec la nouvelle valeur

  int hourAngle = 0;
  int minuteAngle = 0;
  int secondsToReachAngle = 5;
  int rotationMode = 0; //(0=le sens le plus pret pour atteindre la destination,1=sens horaire,2=sens anti-horaire)

  //en fonction de cette valeur, on joue une animation differente 
  //NOUVEAU: (l'aiguille des heures sens horaire, l'autre anti-horaire)
  switch(ten){
    case 0://cas 0 (entre 0 et 9s)
      hourAngle = 0;
      minuteAngle = 0;
      break;
     case 1://cas 1 (entre 10 et 19s)
      hourAngle = 45;
      minuteAngle = 315;
      break;
     case 2://etc...
      hourAngle = 90;
      minuteAngle = 270;
      break;
     case 3:
      hourAngle = 135;
      minuteAngle = 225;
      break;
     case 4:
      hourAngle = 180;
      minuteAngle = 180;
      break;
     case 5:
      hourAngle = 270;
      minuteAngle = 90;
      break;
  }
    //on demande a l'horloge d'aller en fonction des parametres
    clock.setDestination(hourAngle,minuteAngle,secondsToReachAngle,rotationMode);
    
  }
  //a chaque passage de la fonction animate, on anime l'horloge, de manière a ce quelle atteigne sa destination (donnée toutes  les 10s au dessus)
  clock.animate();
}

int getSeconds(){
  unsigned long ms = millis();
  return (ms/1000)%60;
}

int tenPart(){
  return getSeconds()/10;
}
