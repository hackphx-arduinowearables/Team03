#include <Wire.h>
#include <SeeedOLED.h>
#include <TinyGPS++.h>
#define SerialBaud   9600
#define Serial1Baud  9600

TinyGPSPlus gps;
float flat, flon;
unsigned long chars;
unsigned short sentences, failed_checksum;

//Heatsync Labs Latitude and Longitude
const double HEATSYNC_LAT = 33.41532;
const double HEATSYNC_LON = -111.835625;

const uint32_t MAX_AGE = 30000;
const uint32_t UPDATE_INTERVAL = 1000;

extern unsigned char hsllogo[] PROGMEM;

void setup(){
  Serial.begin(SerialBaud);
  Serial1.begin(Serial1Baud);
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;        
  PORTB |= 0x21;

  SeeedOled.clearDisplay();
  SeeedOled.setNormalDisplay();  
  SeeedOled.setPageMode();

  splashscreen("HeeetSeeeker");
}

void loop() 
{
  while (Serial1.available())
  {
    gps.encode(Serial1.read());
  }

  uint32_t age = gps.location.age();

  if (age > MAX_AGE)
  {
    static uint32_t ms = 0;

    if (((!ms) && (millis() > 6000))
      || (millis() - ms > 2000))
    {
      ms = millis();

      static bool b = false;
      if (!(b = !b))
      {
        splashscreen("no GPS...");
      }
      else
      {
        splashscreen("seeeking :)");
      }
    }
  }
  else if (age < UPDATE_INTERVAL)
  {
    //calculate distance in meters*
    double dist = 
        gps.distanceBetween(
            gps.location.lat(),
            gps.location.lng(),
            HEATSYNC_LAT,
            HEATSYNC_LON) / 1000000.0;
       
    //write the text, inform the user
    SeeedOled.setTextXY(0,3);
    SeeedOled.putString("Distance in m");
    SeeedOled.setTextXY(1,3);
    SeeedOled.putString("to HeatSync:");
       
    SeeedOled.setTextXY(2,3);
    SeeedOled.putNumber(dist);
  }
  else
  {
    // Not enough time elapsed for update; don't do anything
  }
}

void splashscreen(const char *s)
{
  SeeedOled.setTextXY(0,0);
  SeeedOled.drawBitmap(hsllogo, 1024);
  SeeedOled.setTextXY(3,8 - (strlen(s) >> 1));
  SeeedOled.putString(s);
}