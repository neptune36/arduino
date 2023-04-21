#include "Arduino.h"
#include "ClockMotor.h"


ClockMotor::ClockMotor(){}
ClockMotor::ClockMotor(byte step_pin,byte direction_pin)
{
	this->step_pin = step_pin;
	this->direction_pin = direction_pin;
	
	pinMode(this->step_pin, OUTPUT);
	pinMode(this->direction_pin, OUTPUT);
}

//clockwiseMode:0 auto; 1 clockwise; 2 counterclockwise
void ClockMotor::setDestination(int degrees, float inSeconds, byte clockwiseMode){
	
	this->clockwiseMode = clockwiseMode;
	//modulo pour ne pas depasser le nombre de steps maximum du moteur
	positionInSteps = positionInSteps % STEPS_PER_REVOLUTION;
	if(positionInSteps<0){
		//si la position en step est inferieur a zero, on recalibre en positif (ex -1024 devient 1024)
		positionInSteps+=STEPS_PER_REVOLUTION;
	}
	if(logging)Serial.println((String)"Position   : "+positionInSteps+" steps");
	destinationInSteps = degreesToSteps(degrees);
	if(logging)Serial.println((String)"Destination: "+destinationInSteps+" steps "+ degrees+" degrees");
	
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
			if(logging)Serial.println((String)"Target reached "+positionInSteps+'/'+destinationInSteps);
			//target  reached
		}
	}
}

void ClockMotor::step(int steps){
	
	int stepFactor = steps>0?1:-1;
	int stepsDone = 0;
	while(stepsDone < abs(steps)){
		//if(logging)Serial.println((String)"step code "+positionInSteps%4);
		digitalWrite(step_pin,LOW);
		digitalWrite(step_pin,HIGH);
		digitalWrite(direction_pin, steps>0 ? LOW : HIGH);
		delayMicroseconds(200);
		
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

