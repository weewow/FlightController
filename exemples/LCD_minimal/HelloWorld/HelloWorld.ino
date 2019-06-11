#include <Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
  lcd.init(); // initialisation de l'afficheur
  lcd.backlight();
  // Envoi du message
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("World :)");
}
void loop(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("World :) .");
  delay(1000);
  //
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("World :) ..");
  delay(1000);
  //
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("World :) ...");
  delay(1000);
}
