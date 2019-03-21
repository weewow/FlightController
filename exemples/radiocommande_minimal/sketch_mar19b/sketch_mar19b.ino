void setup() {
  pinMode(3, INPUT);
  Serial.begin(9600);
}

void loop() {
  int duree(1300);
  int angle(0);
  duree = pulseIn(3, HIGH);
  angle = map(duree, 1170, 2000, 0, 180); 
  if (angle < 200) // Pour éviter la majorité des bruits
    Serial.println(String(" angle : ")+angle+String(" ° | durée mesurée : ")+duree);
}
