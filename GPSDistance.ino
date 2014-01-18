#include <Wire.h>
#include <SeeedOLED.h>
#include <TinyGPS++.h>
#define SerialBaud   9600
#define Serial1Baud  9600

TinyGPSPlus gps;

//Heatsync Labs Latitude and Longitude
const double HEATSYNC_LAT = 33.41532;
const double HEATSYNC_LON = -111.835625;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SerialBaud);
  Serial1.begin(Serial1Baud);
  
  
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;        
  PORTB |= 0x21;
  
  //setup display
  SeeedOled.setNormalDisplay();
  SeeedOled.setPageMode();
  SeeedOled.clearDisplay();

  //Show Heatsync Logo for 3 Seconds here
  
  //start reading data from the GPS
  Wire.begin();
  

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
       SeeedOled.putString("Distance in m"); //Print the String
       SeeedOled.setTextXY(1,3);
       SeeedOled.putString("to HeatSync:");
       
       SeeedOled.setTextXY(2,3);
       SeeedOled.putNumber(dist);

     
    }
 }   
}
