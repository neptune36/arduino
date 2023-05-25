#ifndef Clock_h
#define Clock_h

#include "ClockMotor.h"

class Clock{

	public:
		
		ClockMotor *hourMotor;
		ClockMotor *minuteMotor;

		Clock();
		Clock(byte hall_hour_pin, byte step_hour_pin, byte direction_hour_pin, byte hall_minute_pin, byte step_minute_pin, byte direction_minute_pin);
		void calibrate();
		bool calibrated();
		
		void setDestination(int degreeHour, int degreeMinute, float rotationTimeInSeconds, byte clockwiseMethod);
		void animate();
		void test();
		
		
	private:
		byte hall_hour_pin;
		byte hall_minute_pin;
};

#endif