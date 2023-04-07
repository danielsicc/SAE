#include <CrsfSerial.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GPS.h>
#include <Wire.h>

#define GPSSerial Serial1
#define GPSECHO false
#define ELRSECHO false

uint32_t timer1 = millis();

Adafruit_NeoPixel LED(1, 16, NEO_GRB + NEO_KHZ800);
SerialPIO Receiver(10, 11);
CrsfSerial crsf(Receiver, 200000);
Servo Aileron, Elevator, Motor, Rudder;
Adafruit_GPS GPS(&GPSSerial);

float latitud, longitud, latitudDegs, longitudDegs;

void setup(){
    Serial.begin(9600);
    Serial.println("Init ELRS");
    Receiver.begin(200000);
    Aileron.attach(29);
    Elevator.attach(28);
    Motor.attach(27);
    Rudder.attach(26);
    pinMode(6,OUTPUT_12MA);
    pinMode(7,OUTPUT_12MA);
}

void setup1(){
  Serial.println("Init GPS");
  GPS.begin(9600);
  
  LED.begin();
  LED.clear();
  LED.setPixelColor(0, LED.Color(0, 0, 50));
  LED.show();
}

void loop(){
   control();
}

void loop1(){
  gps();
}

void control(){
  crsf.loop();
  controlFlag=crsf.getChannel(7);
  if(controlFlag>1900){
    Aileron.writeMicroseconds(crsf.getChannel(1));
    Elevator.writeMicroseconds(crsf.getChannel(2));
    Motor.writeMicroseconds(crsf.getChannel(3));
    Rudder.writeMicroseconds(crsf.getChannel(4));
  }
  else{
    Aileron.writeMicroseconds(1500);
    Elevator.writeMicroseconds(1500);
    Motor.writeMicroseconds(1000);
    Rudder.writeMicroseconds(1500);
    //Serial.println("ELRS Inactive");
  }
  if (crsf.getChannel(5)>1500){
    digitalWrite(6,LOW);
  }
  else{
    digitalWrite(6,HIGH);
  }
  if (crsf.getChannel(6)>1500){
    digitalWrite(7,LOW);
  }
  else{
    digitalWrite(7,HIGH);
  }
  if (ELRSECHO){
  Serial.print(crsf.getChannel(1));
  Serial.print(" ");
  Serial.print(crsf.getChannel(2));
  Serial.print(" ");
  Serial.print(crsf.getChannel(3));
  Serial.print(" ");
  Serial.print(crsf.getChannel(4));
  Serial.print(" ");
  Serial.print(crsf.getChannel(5));
  Serial.print(" ");
  Serial.print(crsf.getChannel(6));
  Serial.print(" ");
  Serial.println(crsf.getChannel(7));
  }
}

void gps(){
  char c = GPS.read();
  if (GPSECHO){
    if (c) Serial.print(c);
  }
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }

  if (millis() - timer2 > 1000) {
    timer1 = millis();
    Serial.println("");
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" Quality: "); Serial.print((int)GPS.fixquality);
    Serial.print(" Satellites: "); Serial.println((int)GPS.satellites);
    if (GPS.fix) {
      Serial.print("Location (DD MM): "); 
      Serial.print(GPS.latitude, 6); Serial.print(GPS.lat); Serial.print(", ");
      Serial.print(GPS.longitude, 6); Serial.println(GPS.lon);
      latitud = GPS.latitude;
      longitud = GPS.longitude;
      convCords();
      Serial.print("Location (DD): "); 
      Serial.print(latitudDegs, 6); Serial.print(", ");
      Serial.println(longitudDegs, 6);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      distObj = objetivo(latitudDegs, longitudDegs, latObj, lonObj);
      Serial.println(""); 
    }
  }
}

void convCords(){
 //Initialize the location.
 float lat = latitud;
 float lon = longitud;
 // Get the first two digits by turning f into an integer, then doing an integer divide by 100;
 int firsttwoLat = ((int)lat)/100; //This assumes that f < 10000.
 int firsttwoLon = ((int)lon)/100; //This assumes that f < 10000.
 float nexttwoLat = lat - (float)(firsttwoLat*100);
 float nexttwoLon = lon - (float)(firsttwoLon*100);
 latitudDegs=(float)(firsttwoLat + nexttwoLat/60.0);
 longitudDegs=-(float)(firsttwoLon + nexttwoLon/60.0);
}
