#include<DHT.h>                     //Library for using DHT sensor 
#define DHTPIN PA15
#define DHTTYPE DHT11
 
DHT dht(DHTPIN, DHTTYPE);     //initilize object dht for class DHT with DHT pin with STM32 and DHT type as DHT11
 
 float h = 0;
 float t = 0;
 float f = 0;
 
void setup()
{
  Serial.begin(9600);
  dht.begin();                                  
}
 
void loop()
{
    if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
   h = dht.readHumidity();       //Gets Humidity value
   t = dht.readTemperature();    //Gets Temperature value
  Serial.print(t);
  Serial.print("     |     ");
  Serial.println(h);
  delay(2000);
}
