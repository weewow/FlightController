#include <Servo.h>

Servo myservo;  
int sleeptime = 1000;  

void setup() {
  pinMode(3, INPUT);
  pinMode(6, OUTPUT);
  //Serial.begin(9600);
  myservo.attach(6);  // D6
}

void loop() {
  int duree(1300);
  int angle(0);
  duree = pulseIn(3, HIGH);
  angle = map(duree, 1170, 2000, 0, 180);  
  if (angle < 200){ // Pour éviter la majorité des bruits
    myservo.write(angle);
    delay(200);
    // Serial.println(String(" angle : ")+angle+String(" ° | durée mesurée : ")+duree);
  }
}
