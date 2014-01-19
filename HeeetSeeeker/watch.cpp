#include <Arduino.h>
#include <TinyGPS++.h>
#include <SeeedOLED.h>

extern TinyGPSPlus gps;
extern SeeedOLED SeeedOled;

String timeString;

void watch() {
    //sloppy way to get correct time zone
   int hour = gps.time.hour() - 7;
   
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
