#include <LiquidCrystal.h>

LiquidCrystal lcd(PB9, PB8, PB7, PB6, PB5, PB4);

void setup()
{
  lcd.begin(16, 4);  
  pinMode(PC13, OUTPUT);
  Serial1.begin(9600);
  analogWrite(PA8,100);
  lcd.print("===SMART HEMS===");
}

void loop() 
{
  if(Serial1.available()>0) 
  {}
  char btVal = Serial1.read();
  
  if (btVal == 'a')
  {
    digitalWrite(PC13,LOW);
    lcd.setCursor(-4, 2);
    lcd.print("LED is turned ON");
    delay(1000);
  }
  if (btVal == 'b')
  {
    digitalWrite(PC13,HIGH);
    lcd.setCursor(-4, 2);
    lcd.print("LED is turned OFF");
    delay(1000);
  }
  
lcd.setCursor(0, 1);
lcd.print("Counter: ");
lcd.print(counter);
lcd.setCursor(-4, 2);
lcd.print("Temp.: ");
lcd.print(t);
lcd.setCursor(-4, 3);
lcd.print("Hum.: ");
lcd.print(h);
}
