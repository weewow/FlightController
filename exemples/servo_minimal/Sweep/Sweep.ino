#include <Servo.h>

Servo myservo;  
int sleeptime = 1000;    

void setup() {
  myservo.attach(6);  // D6
}

void loop() {
  myservo.write(90);        
  delay(sleeptime);
  myservo.write(179);        
  delay(sleeptime);
  myservo.write(90);        
  delay(sleeptime);
  myservo.write(1);        
  delay(sleeptime);
//myservo.write(90);
}
