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
const uint32_t UPDATE_INTERVAL = 5000;

extern unsigned char hsllogo[] PROGMEM;

void watch();

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
  else if (gps.location.isUpdated())
  {
    //calculate distance in meters*
    double dist = 
        gps.distanceBetween(
            gps.location.lat(),
            gps.location.lng(),
            HEATSYNC_LAT,
            HEATSYNC_LON);

    String s;
    
    s.concat((unsigned)dist);
    s += "meters";
    char buf[17];
    s.toCharArray(buf, sizeof(buf));
    centertext(3, buf);
    
    centertext(4, "from HSL");
  }
  else
  {
    // Nothing to update
  }
  
  if (gps.time.isUpdated())
  {
    watch();
  }
}

void centertext(unsigned line, const char *s)
{
  SeeedOled.setTextXY(line,8 - (strlen(s) >> 1));
  SeeedOled.putString(s);
}

void splashscreen(const char *s)
{
  SeeedOled.setTextXY(0,0);
  SeeedOled.drawBitmap(hsllogo, 1024);
  centertext(3, s);
}

