#include "Arduino.h"
#include "ClockRtc.h"


String ClockRtc::formatDateHour(byte toFormat){

   int intValue = int(toFormat);
   String formatted = "";
   String decChar = intValue <10 ? "0" : "";
 
  formatted = decChar + intValue;

  return formatted;
  
}

String ClockRtc::getTime(){
  
  return formatDateHour(Clock.getHour(h12, PM))+":"+
  formatDateHour(Clock.getMinute());
 	
}

void ClockRtc::setTime(){


	
}