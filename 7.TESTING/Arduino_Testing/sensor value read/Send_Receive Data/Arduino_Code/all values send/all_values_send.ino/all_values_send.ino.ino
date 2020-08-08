#include<SoftwareSerial.h>

#define RELAYLight 7
#define RELAYFan 6

char btVal;

void setup() 
{
  Serial.begin(9600);
  pinMode(RELAYLight, OUTPUT);
  pinMode(RELAYFan, OUTPUT); 
}

void loop() 
{
  btVal = Serial.read();
  
  if(btVal == 'a')
  {
      digitalWrite(RELAYLight, HIGH);
  } 
   
  if(btVal == 'b')
  {
      digitalWrite(RELAYLight, LOW);
  }  
  
  if(btVal == 'c')
  {
      digitalWrite(RELAYFan, HIGH);
  }  
  
  if(btVal == 'd')
  {
      digitalWrite(RELAYFan, LOW);
  }
  
    long temp = random(20, 45);
    long hum = random(35, 55);
    long counter = random(1, 5);
    long fanamp = random(400, 600);
    long fanpow = random(20, 40);
    long lampamp = random(300, 500);
    long lamppow = random(10, 30);
    long frgamp = random(10, 30);
    long frgpow = random(10, 30);
    long cost = random(10, 30);

  
  Serial.print(temp);
  Serial.print("|");
  Serial.print(hum);
  Serial.print("|");
  Serial.print(counter);
  Serial.print("|");
  Serial.print(fanamp);
  Serial.print("|");
  Serial.print(fanpow);
  Serial.print("|");
  Serial.print(lampamp);
  Serial.print("|");
  Serial.print(lamppow);
  Serial.print("|");
  Serial.print(frgamp);
  Serial.print("|");
  Serial.print(frgpow);
  Serial.print("|");
  delay(1000);
}
