#include <CrsfSerial.h>
#include <Servo.h>
#include <Wire.h>

SerialPIO Receiver(10, 11);
CrsfSerial crsf(Receiver, 200000);
Servo Aileron, Elevator, Motor, Rudder;

void setup(){
  Serial.begin();
    Receiver.begin(200000);
    Serial.begin(9600);
    Aileron.attach(29);
    Elevator.attach(28);
    Motor.attach(27);
    Rudder.attach(26);
    pinMode(7, OUTPUT);
    pinMode(6, OUTPUT);
}

void loop(){
    crsf.loop();
    Aileron.writeMicroseconds(crsf.getChannel(1));
    Elevator.writeMicroseconds(crsf.getChannel(2));
    Motor.writeMicroseconds(crsf.getChannel(3));
    Rudder.writeMicroseconds(crsf.getChannel(4));
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
  
}
