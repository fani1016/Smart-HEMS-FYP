#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() 
{
  Serial.begin(9600); //Start Serial Monitor to display current read value on Serial monitor
}

void loop() 
{
  unsigned int x=0;
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

  for (int x = 0; x < 150; x++)
  { //Get 150 samples
    AcsValue = analogRead(A0);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
  }
  AvgAcs=Samples/150.0;//Taking Average of Samples
  
  AcsValueF = (2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.185;
  
  lcd.setCursor(0,0);
  lcd.print(AcsValueF);//Print the read current on Serial monitor
  
  delay(50);
}
