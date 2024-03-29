#include "Clock.h"
Clock::Clock(){}
/*
*hour: hall,step,direction
*minutes: hall,step,direction
*/
Clock::Clock(byte hall_hour_pin, byte step_hour_pin, byte direction_hour_pin, byte hall_minute_pin, byte step_minute_pin, byte direction_minute_pin){
	this->hall_hour_pin = hall_hour_pin;
	this->hall_minute_pin = hall_minute_pin;
	
	pinMode(this->hall_hour_pin, INPUT_PULLUP);
	pinMode(this->hall_minute_pin, INPUT_PULLUP);
	
	hourMotor = new ClockMotor(step_hour_pin,direction_hour_pin);
	minuteMotor = new ClockMotor(step_minute_pin,direction_minute_pin);
	
	
}

void Clock::calibrate(){

	if(!hourMotor->calibrated){
		if(digitalRead (hall_hour_pin)==HIGH){
			hourMotor->step(1);
		}else{
			hourMotor->reset();
			hourMotor->calibrated = true;
		}
	}
	if(!minuteMotor->calibrated){
		if(digitalRead (hall_minute_pin)==HIGH){
			minuteMotor->step(1);
		}else{
			minuteMotor->reset();
			minuteMotor->calibrated = true;
		}
	}
}

void Clock::test(){
	hourMotor->step(1);
	minuteMotor->step(1);
}

bool Clock::calibrated(){
	return hourMotor->calibrated && minuteMotor->calibrated;
}

void Clock::setDestination(int degreeHour, int degreeMinute, float rotationTimeInSeconds, byte clockwiseMethod){
	hourMotor->setDestination(degreeHour,rotationTimeInSeconds,clockwiseMethod);
	minuteMotor->setDestination(degreeMinute,rotationTimeInSeconds,clockwiseMethod);
}
void Clock::animate(){
	hourMotor->rotate();
	minuteMotor->rotate();
}