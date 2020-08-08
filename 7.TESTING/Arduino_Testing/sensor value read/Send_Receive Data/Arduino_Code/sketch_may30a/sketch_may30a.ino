#include <SimpleDHT.h>

int pinDHT11 = A0;
SimpleDHT11 dht11;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  byte temperature = 0;
  byte humidity = 0;
  
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL))
  {
    return;
  }
  Serial.print((int)temperature);
  Serial.print("|");
  Serial.print((int)humidity);
  Serial.print("|");
  delay(1000);
}
