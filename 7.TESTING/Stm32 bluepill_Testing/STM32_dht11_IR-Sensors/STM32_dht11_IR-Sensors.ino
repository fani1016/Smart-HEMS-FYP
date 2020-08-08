#include "DHT.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(PB9, PB8, PB7, PB6, PB5, PB4);

#define DHTPIN PB3  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define in PB14
#define out PB15

int counter = 0;
char btVal = NULL;
int t = 0;
int h = 0;

void setup()
{
  lcd.begin(16, 4);  
  Serial1.begin(9600);
  dht.begin();
  pinMode(DHTPIN, INPUT); 
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);
  analogWrite(PA8,100);
  lcd.print("===SMART HEMS===");
  delay(3000);
}

void loop() 
{
  if(Serial1.available()>0) {}
  btVal = Serial1.read();
  
  t = dht.readTemperature();
  h = dht.readHumidity();

  if(digitalRead(in))
  {
   counter++;
   delay(800);
  }
  if(digitalRead(out))
  {
    if (counter>0)
    {
      counter--;
    }
   delay(800);
  } 
  if (btVal == 'a')
  {
    digitalWrite(PC13, LOW);
  }
  if (btVal == 'b')
  {
    digitalWrite(PC13, HIGH);
  }

  int cost = random(20, 30);
  int lampA = random(1, 10);
  int lampk = random(5, 15);
  int fanA = random(1, 10);
  int fank = random(5, 15);
  int lampb = random(10, 20);
  int fanb = random(10, 20);
  
  Serial1.print(t);  // #01
  Serial1.print("|");
  Serial1.print(h);  // #02
  Serial1.print("|");
  Serial1.print(counter);  // #03
  Serial1.print("|");
  Serial1.print(cost);  // #04
  Serial1.print("|");
  Serial1.print(lampA);  // #05
  Serial1.print("|");
  Serial1.print(lampk);  // #06
  Serial1.print("|");
  Serial1.print(fanA);  // #07
  Serial1.print("|");
  Serial1.print(fank);  // #08   
  Serial1.print("|");
  Serial1.print(lampb);  // #09   
  Serial1.print("|");  
  Serial1.print(fanb);  // #10   
  Serial1.print("|");
  
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
