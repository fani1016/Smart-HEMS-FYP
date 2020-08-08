#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

  const int currentPin1 = A0;
  
  int sensitivity = 185;
  int offsetVoltage = 2500;
  int adcValue1 = 0;
  double adcVoltage1 = 0;
  double currentValue1 = 0;
  double powerValue1 = 0;
  
void setup() 
{     
  Serial.begin(9600);
  lcd.clear();
  lcd.begin(16, 4);
  lcd.setCursor(0,0);
  lcd.print("ACS-712 Sensors");
}
 
void loop()
{  
  adcValue1 = analogRead(currentPin1);
  adcVoltage1 = (adcValue1 / 1024.0) * 5000;
  currentValue1 = ((adcVoltage1 - offsetVoltage) / sensitivity);
  powerValue1 = currentValue1 * 12;

  lcd.setCursor(0,1);
  lcd.print("Current1 = ");
  lcd.print(currentValue1);
  lcd.print("A");
  lcd.setCursor(0,2);
  lcd.print("Power1 = ");
  lcd.print(powerValue1);
  lcd.print("W");
}
