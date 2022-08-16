#include "Arduino.h"
#include "ClockMotor.h"


ClockMotor::ClockMotor(){}
ClockMotor::ClockMotor(byte motor_pin_1,byte motor_pin_2,byte motor_pin_3,byte motor_pin_4)
{
	this->motor_pin_1 = motor_pin_1;
	this->motor_pin_2 = motor_pin_2;
	this->motor_pin_3 = motor_pin_3;
	this->motor_pin_4 = motor_pin_4;
	
	pinMode(this->motor_pin_1, OUTPUT);
	pinMode(this->motor_pin_2, OUTPUT);
	pinMode(this->motor_pin_3, OUTPUT);
	pinMode(this->motor_pin_4, OUTPUT);
}

void ClockMotor::setDestination(int degrees, float inSeconds, byte clockwiseMode){
	
	this->clockwiseMode = clockwiseMode;
	//modulo pour ne pas depasser le nombre de steps maximum du moteur
	positionInSteps = positionInSteps % STEPS_PER_REVOLUTION;
	if(positionInSteps<0){
		//si la position en step est inferieur a zero, on recalibre en positif (ex -1024 devient 1024)
		positionInSteps+=STEPS_PER_REVOLUTION;
	}
	if(logging)Serial.println((String)"Position before move: "+positionInSteps);
	destinationInSteps = degreesToSteps(degrees);
	if(logging)Serial.println((String)"Set destination: "+destinationInSteps+" steps "+ degrees+" degrees");
	
	if(clockwiseMode==0){//si mode auto, chemin le plus court
		int distanceA = (STEPS_PER_REVOLUTION+positionInSteps-destinationInSteps)%STEPS_PER_REVOLUTION;
		int distanceB = (STEPS_PER_REVOLUTION+destinationInSteps-positionInSteps)%STEPS_PER_REVOLUTION;
		clockwise = distanceA>distanceB;
	}else{//sinon on force le sens
		clockwise = clockwiseMode==1;
	}
	if(logging)Serial.println((String)"Clockwise: "+clockwise);
	//calcul de la distance en step a parcourir
	distanceInSteps = abs(distanceToGo());
	if(logging)Serial.println((String)"Distance to go: "+distanceInSteps);
	stepsPerMilliseconds = distanceToGo() / inSeconds / 1000;
	stepsPerMilliseconds = abs(stepsPerMilliseconds);
	if(logging)Serial.println((String)"Steps per millis: "+stepsPerMilliseconds);

	startMillis = millis();
	stepsDone = 0;
}

void ClockMotor::rotate()
{
	if(abs(distanceToGo())>0){
		//nb de millis secondes ecoulées depuis setDestination
		float amountOfMillis = millis()-startMillis;
		//nb de steps ayant du etre fait depuis setDestination
		float stepsTotal = amountOfMillis * stepsPerMilliseconds;
		//on bride le nb de steps
		if(stepsTotal>distanceInSteps){
			stepsTotal = distanceInSteps;
		}
		int stepsInt = (int) stepsTotal;
		//le nb de step a faire maintenant est egal au nb de steps ayant du etre fait, moins ceux deja fait
		int stepsToDoNow = stepsInt - stepsDone;
		//if(logging)Serial.println((String)"Steps to do now "+stepsToDoNow);
		if(stepsToDoNow > 0){// si il y en a a faire, on les fait
			step(stepsToDoNow * (clockwise ? 1 : -1));
			stepsDone += stepsToDoNow;
		}
		
		if(stepsInt >= distanceInSteps){
			//if(logging)Serial.println((String)"Target reached "+positionInSteps+'/'+destinationInSteps);
			//target  reached
		}
	}
}

void ClockMotor::step(int steps){
	int stepFactor = steps>0?1:-1;
	int stepsDone = 0;
	//if(logging)Serial.println((String)"step clockwise "+clockwise);
	while(stepsDone < abs(steps)){
		//if(logging)Serial.println((String)"step code "+positionInSteps%4);
		switch(abs(positionInSteps)%4){
			case 3:
				digitalWrite(motor_pin_1,HIGH);
				digitalWrite(motor_pin_2,LOW);
				digitalWrite(motor_pin_3,HIGH);
				digitalWrite(motor_pin_4,LOW);
				break;
			case 2:
				digitalWrite(motor_pin_1,LOW);
				digitalWrite(motor_pin_2,HIGH);
				digitalWrite(motor_pin_3,HIGH);
				digitalWrite(motor_pin_4,LOW);
				break;
			case 1:
				digitalWrite(motor_pin_1,LOW);
				digitalWrite(motor_pin_2,HIGH);
				digitalWrite(motor_pin_3,LOW);
				digitalWrite(motor_pin_4,HIGH);
				break;
			case 0:
				digitalWrite(motor_pin_1,HIGH);
				digitalWrite(motor_pin_2,LOW);
				digitalWrite(motor_pin_3,LOW);
				digitalWrite(motor_pin_4,HIGH);
				break;
		}
		stepsDone++;
		positionInSteps += stepFactor;
		positionInSteps = positionInSteps % STEPS_PER_REVOLUTION;

	if(positionInSteps<0){
		//si la position en step est inferieur a zero, on recalibre en positif (ex -1024 devient 1024)
		positionInSteps+=STEPS_PER_REVOLUTION;
	}
	}
}
void ClockMotor::reset(){
	positionInSteps=0;
}
int ClockMotor::degreesToSteps(int degree){
  return (float)STEPS_PER_REVOLUTION / 360 * degree;
}

int ClockMotor::distanceToGo(){
	if(clockwise){
		return (STEPS_PER_REVOLUTION+destinationInSteps-positionInSteps)%STEPS_PER_REVOLUTION;
	}else{
		return (STEPS_PER_REVOLUTION+positionInSteps-destinationInSteps)%STEPS_PER_REVOLUTION;
	}
}

