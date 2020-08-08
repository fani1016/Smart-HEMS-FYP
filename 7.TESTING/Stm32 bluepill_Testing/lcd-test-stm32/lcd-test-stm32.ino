#include <LiquidCrystal.h> // initialize the library with the numbers of the interface pins – The numbers
LiquidCrystal lcd(PB9, PB8, PB7, PB6, PB5, PB4);

void setup()
{
lcd.begin(16, 4);
analogWrite(PA8,100);
lcd.print("-----F.T.J-----");
}

void loop() 
{
lcd.setCursor(0, 1);
lcd.print("     Farhan     ");
lcd.setCursor(-4, 2);
lcd.print("     Tariq     ");
lcd.setCursor(-4, 3);
lcd.print("     Janjua     ");
}
