#ifndef ClockBluetooth_h
#define ClockBluetooth_h

#include <SoftwareSerial.h>


class ClockBluetooth{

	private:
	SoftwareSerial *hc05;
	void readBluetooth();
	static void (*user_onReceive)(String);

	public:
	ClockBluetooth(uint8_t rxPin,uint8_t txPin);
	void loop();
	void sendMessage(String message);
	void onReceive(void (*)(String)); 
	
	
	
};

#endif