#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int sensorIn = A0;

int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
float PF = 0.80;
float energyTariff = 8.0; // Energy cost in PKR per unit (kWh)

unsigned long last_time =0;
unsigned long current_time =0;

float KWh =0 ;             // Energy in kWh
double bill_amount = 0;

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
double Watt = 0;

void setup()
{ 
 lcd.begin(16,4);
}

void loop()
{
 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707; 
 AmpsRMS = (VRMS * 1000) / mVperAmp;   
 Watt = (AmpsRMS * 240) * PF;

 last_time = current_time;
 current_time = millis();    
 KWh = KWh +  Watt *(( current_time - last_time) /3600000.0) ; // calculating energy in Watt-Hour
 bill_amount = KWh * (energyTariff/1000);


 lcd.setCursor(0,0);
 lcd.print("Amp.: ");
 lcd.print(AmpsRMS);
 lcd.println(" A");
 
 lcd.setCursor(0,1);
 lcd.print("Power: ");
 lcd.print(Watt);
 lcd.println(" W");
 
 lcd.setCursor(0,2);
 lcd.print("Energy: ");
 lcd.print(KWh);
 lcd.println(" KWh");
 
 lcd.setCursor(0,3);
 lcd.print("Bill: ");
 lcd.print(bill_amount);
 lcd.println(" PKR");
  
}

float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 
 }
