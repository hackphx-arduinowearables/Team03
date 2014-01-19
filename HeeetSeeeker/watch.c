#include <Arduino.h>
#include <TinyGPS++.h>
#include <SeeedOLED.h>

extern TinyGPSPlus gps;
extern SeeedOLED SeeedOled;

String timeString;

void watch() {
   int hour = gps.time.hour();
   int minute = gps.time.minute();
   int second = gps.time.second();
   
   timeString += hour;
   timeString += ":";
   timeString += minute;
   timeString += ".";
   timeString += second;
   
   SeeedOled.setTextXY(0,2);
   SeeedOled.putString(timeString)
}
