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

extern unsigned char hsllogo[] PROGMEM;

void setup(){
  Serial.begin(SerialBaud);
  Serial1.begin(Serial1Baud);
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;        
  PORTB |= 0x21;

  SeeedOled.clearDisplay();
  SeeedOled.drawBitmap(hsllogo, 1024);

  SeeedOled.setNormalDisplay();  
  SeeedOled.setPageMode();
  SeeedOled.setTextXY(3,3);
  SeeedOled.putString("HeatSeeker");
  
  delay(1000);
  
  SeeedOled.clearDisplay();
}

void loop() {
  if (Serial1.available()){
    if(gps.encode(Serial1.read())){
      //every second, update the distance
      delay(1000);
      
      
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
 }   
}

