/*
int pin = 3;

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  noInterrupts();
  int duree(0);
  int angle(0);
  duree = pulseIn(pin, HIGH);
  //angle = map(duree, 1170, 2000, 0, 180);
  if ((duree < 2000) and (duree > 1000))
  {
    Serial.println(duree); 
    //if ((angle < 200) && (angle > 0)) // Pour éviter la majorité des bruits
    //  Serial.println(String(" angle : ")+angle+String(" ° | durée mesurée : ")+duree);
    delay(50);
  }
}
*/

byte receiverPin = 3;
unsigned long duration;

void setup()
{
  Serial.begin(115200);
  pinMode(receiverPin, INPUT);
}

void loop()
{
  duration = pulseIn(receiverPin, HIGH);
  if ((duration < 10000) and (duration > 2000))
    Serial.println(duration);
  delay(100);
}
