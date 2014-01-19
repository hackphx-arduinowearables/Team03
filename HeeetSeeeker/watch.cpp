/*
    This module contains the implementation of the function that shows the
    time on the screen.

    Separating this into a different module made it possible for more than
    one person to work on the code without having to merge changes done in 
    the same source file.
*/

#include <Arduino.h>
#include <TinyGPS++.h>
#include <SeeedOLED.h>

extern TinyGPSPlus gps;
extern SeeedOLED SeeedOled;

String timeString;

void watch() {
   // Subtract 7 hours from UTC to get Mountain Time with no DST
   int hour = ((gps.time.hour() - 7) + 24) % 24;
   
   int minute = gps.time.minute();
   int second = gps.time.second();
   
   // Generate 12-hour time
   timeString += (hour == 0 ? 12 : (hour > 12 ? hour - 12 : hour));
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
