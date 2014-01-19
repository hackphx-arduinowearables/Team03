/*
    HeeetSeeeker is a Xadow-based wearable wristband that's indispensable
    to keep track of how far your are from HeatSync Labs.

    The name was inspired by the SeeedStudio name, and is also extra
    appropriate because we are Team Threee :-)

    TO DO (i.e. things we didn't finish):
    - Get the current direction from the GPS and determine whether to turn
      left or right, based on the vector <current location - HSL location>
    - Use e.g. the Arduino Time library to let the GPS update the system
      clock (so that the time on the screen will still be updated when the
      user loses GPS connectivity) and to do proper calculations of local
      time (based on time zone).
    - Show the time on the screen in a fancier way (e.g. using graphics
      instead of text)
    - Use a Blutooth module and an app on an iPhone or Android phone to track
      where your phone is
    - Use an NFC module and some more hardware to track where the nearest
      NFC-wielding zombies are, etc.
*/

#include <Wire.h>
#include <SeeedOLED.h>
#include <TinyGPS++.h>
#define SerialBaud   9600
#define Serial1Baud  9600

TinyGPSPlus gps;

// Heatsync Labs Latitude and Longitude
const double HEATSYNC_LAT = 33.41532;
const double HEATSYNC_LON = -111.835625;

// If we don't receive GPS for this many ms, display a message
const uint32_t MAX_AGE = 30000;

// Declaration for bitmap data in other module
extern unsigned char hsllogo[] PROGMEM;

// External function to show time, hopefully in a fancy way :-)
void watch();

// Forward declaration needed because of the default parameter
void splashscreen(const char *s, bool force = true);

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
    //calculate distance in meters
    double dist = 
        gps.distanceBetween(
            gps.location.lat(),
            gps.location.lng(),
            HEATSYNC_LAT,
            HEATSYNC_LON);

    String s;

    if (dist >= 1000.0)
    {
      s.concat((unsigned)(dist / 1000.0));
      s += " km";
    }
    else if (dist >= 20.0)
    {
      s.concat((unsigned)dist);
      s += " meters";
    }
    else
    {
      s = "WELCOME";
    }

    char buf[17];
    s.toCharArray(buf, sizeof(buf));
    
    splashscreen(buf, false);
    
    centertext(4, "to HeatSync Labs");
  }
  else
  {
    // Nothing to update
  }
  
  if (gps.time.isUpdated())
  {
    // Show the time
    watch();
  }
}

void centertext(unsigned line, const char *s)
{
  SeeedOled.setTextXY(line,8 - (strlen(s) >> 1));
  SeeedOled.putString(s);
}

void splashscreen(const char *s, bool force /* = true */)
{
  // To reduce flicker, don't redraw the screen if the text in the
  // previous call was shorter than the text in the current call
  // (unless a redraw is forced)
  static unsigned prevlen;
  unsigned len = strlen(s);
  if ((force) || (len < prevlen))
  {
    SeeedOled.setTextXY(0,0);
    SeeedOled.drawBitmap(hsllogo, 1024);
    
    prevlen = len;
  }
  
  centertext(3, s);
}


