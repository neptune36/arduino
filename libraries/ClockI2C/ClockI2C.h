#ifndef ClockI2C_h
#define ClockI2C_h

#include <Wire.h>


class ClockI2C{

	private:
	String readI2C(int from, int expectedLength);
	void writeI2C(int destination, String message);

	public:
	ClockI2C();
	String askQuestionTo(int destination,String question, int responseLength);
	
	
	
};

#endif