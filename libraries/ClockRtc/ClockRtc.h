#ifndef ClockRtc_h
#define ClockRtc_h

#include <DS3231.h>


class ClockRtc{

	private:
	DS3231 *clock;
	formatDateHour(byte toFormat);

	public:
	String getTime();
	void setTime(String newTime);

};

#endif