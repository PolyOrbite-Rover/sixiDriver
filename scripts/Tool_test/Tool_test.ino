#include <Servo.h> 
int servoPin = 12; 
Servo Servo1; 

void setup() {
  Servo1.attach(servoPin); 
 
}

void loop() {
  // put your main code here, to run repeatedly:
// Make servo go to 0 degrees 
   Servo1.write(90); 
   delay(2000); 
   // Make servo go to 90 degrees 
   Servo1.write(0); 
   delay(1500); 
   // Make servo go to 180 degrees 
    Servo1.write(180); 
   delay(1500); 
}