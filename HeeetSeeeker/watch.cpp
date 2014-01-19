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
   if (minute < 10) timeString += '0';
   timeString += minute;
   timeString += ".";
   if (second < 10) timeString += '0';
   timeString += second;
   
   SeeedOled.setTextXY(0,2);

   char buf[9];
   timeString.toCharArray(buf, sizeof(buf));
   SeeedOled.putString(buf);
}
