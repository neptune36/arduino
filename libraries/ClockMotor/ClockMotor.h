#ifndef ClockMotor_h
#define ClockMotor_h

#include "Arduino.h"

class ClockMotor
{
	public:
		
		ClockMotor();
		ClockMotor(byte step_pin,byte direction_pin);
		void setDestination(int degree, float inSeconds,byte clockwiseMode);
		void rotate();
		void step(int steps);
		void reset();
		
		bool logging;
		bool calibrated=false;
		
		
	private:
		byte step_pin;
		byte direction_pin;
		
		int destinationInSteps;
		int const STEPS_PER_REVOLUTION = 4320;
		float stepsPerMilliseconds;
		float startMillis;
		int stepsDone;
		int distanceInSteps;
		byte clockwiseMode;
		bool clockwise;
		int positionInSteps;

		int degreesToSteps(int degree);
		int distanceToGo();
};

#endif