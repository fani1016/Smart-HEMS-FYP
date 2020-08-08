/*  Written By: Farhan Tariq Janjua
 *  B.Sc Electrical Engineering
 *  www.APCOMS.edu.pk
  ---------------------------------------------------
  -Set Lamp Energy Limit in Controlling Lamp Section.
  -Set Fan Energy Limit in Controlling Fan Section.
*/

#include "DHT.h"
#include "LiquidCrystal.h"
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

#define DHTPIN 13  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define RELAYLight 9
#define RELAYFan 10
#define in 11
#define out 12

int counter=0;
int islightOn=0;
int isFanOn=0;
char btVal=NULL;
int t = 0;
int h = 0;

/* ========Current Circuit variables definition======== */

#define ACS_lamp 15  
#define ACS_Fan 16  

int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
float PF = 0.80;
float energyTariff100 = 10.0; // Energy cost in PKR per unit (kWh) below 100 units
float energyTariff200 = 15.0; // Energy cost in PKR per unit (kWh) above 100 units
unsigned long last_time1 =0;
unsigned long current_time1 =0;
float Voltage1 = 0;
float VRMS1 = 0;
float LampAmp = 0;
float LampPow = 0;
float LampKWh =0;             
float LampBill = 0;
int Lamp_Wh = 0;
int Lamp_mAmp = 0;
float Lampcost = 0; 
float NewLampBill = 0; 
float Total_Cost = 0;
unsigned long last_time2 =0;
unsigned long current_time2 =0;
float Voltage2 = 0;
float VRMS2 = 0;
float FanAmp = 0;
float FanPow = 0;
float FanKWh =0;             
float FanBill = 0;
int Fan_Wh = 0;
int Fan_mAmp = 0;
float Fancost = 0; 
float NewFanBill = 0;

void setup()
{
  lcd.begin(16,4);
  analogWrite(6,100);
  Serial.begin(9600);
  dht.begin();
  pinMode(DHTPIN, INPUT); 
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(ACS_lamp, INPUT);
  pinMode(ACS_Fan, INPUT);
  pinMode(RELAYLight, OUTPUT);
  pinMode(RELAYFan, OUTPUT); 
  digitalWrite(RELAYLight, LOW);
  digitalWrite(RELAYFan, LOW); 
  lcd.print("===Smart HEMS===");
  delay(3000);
      
  attachInterrupt(digitalPinToInterrupt(11),IN,RISING); 
  attachInterrupt(digitalPinToInterrupt(12),OUT,RISING); 
  attachInterrupt(digitalPinToInterrupt(1),BTI,RISING); 
}

void loop()
{  
/*=====Temp. and Humidity Sensing=====*/
  t = dht.readTemperature();
  h = dht.readHumidity();

  if(Serial.available()>0) {}
  
/*=====Bluetooth Receiving=====*/
  BTI();
  
/*=====Presence Sensing=====*/
  if(digitalRead(in))
  {
   IN();
   delay(250);
  }
  if(digitalRead(out))
  {
   OUT();
   delay(250);
  } 
  
if(counter>0)
{  
  if (islightOn == 0)
  {
      if (btVal == 'a' || (counter > 0 && btVal == NULL))
      {
        digitalWrite(RELAYLight, HIGH);
        islightOn = 1;
      } 
  }
  if (islightOn == 1)
  {
      if(btVal == 'b' || (counter == 0 && btVal == NULL))
      {
        digitalWrite(RELAYLight, LOW);
        islightOn = 0;
      } 
  }

  if (isFanOn == 0)
  {
      if (btVal == 'c' || (t >= 35 && btVal == NULL))
        {
        digitalWrite(RELAYFan, HIGH);
        isFanOn = 1;
      }
  }
  
  if (isFanOn == 1)
  {
      if (btVal == 'd' || (t < 35 && btVal == NULL))
      {
        digitalWrite(RELAYFan, LOW);
        isFanOn = 0;
      }
  }
}

  if(counter == 0)
  {
      digitalWrite(RELAYLight, LOW);
      islightOn = 0;  
      digitalWrite(RELAYFan, LOW);
      isFanOn = 0;  
  }
  
  if(islightOn == 0 && isFanOn == 0)
  {
    Total_Cost = Total_Cost;
    LampBill = LampBill;
    FanBill = FanBill;
  } 
  if(islightOn == 1)
  {
    calcLamp();
    Total_Cost = Total_Cost + LampBill;
  }
  if(isFanOn == 1)
  {
    calcFan();
    Total_Cost = Total_Cost + FanBill;
  } 

  
/* ========Bluetooth Data Transmit======== */
  Serial.print(t);  // #01
  Serial.print("|");
  Serial.print(h);  // #02
  Serial.print("|");
  Serial.print(counter);  // #03
  Serial.print("|");
  Serial.print(Total_Cost);  // #04
  Serial.print("|");
  Serial.print(Lamp_mAmp);  // #05
  Serial.print("|");
  Serial.print(Lamp_Wh);  // #06
  Serial.print("|");
  Serial.print(Fan_mAmp);  // #07
  Serial.print("|");
  Serial.print(Fan_Wh);  // #08   
  Serial.print("|");
  Serial.print(LampBill);  // #09   
  Serial.print("|");  
  Serial.print(FanBill);  // #10   
  Serial.print("|");
  
  /* ========Counter/Temp/Hum Data Display======== */
  Disp_All();
  delay(2000);
  Disp_Lamp();
  delay(2000);
  Disp_Fan();
  delay(1500);
}

/* ========Bluetooth Interrupt======== */
void BTI()
{ 
  btVal = Serial.read();
}

/* ========IR Sensors Interrupts======== */
void IN()
{
  counter++;   
  Disp_All();
}

void OUT()
{
  if (counter > 0)
  {
    counter--; 
  }     
    Disp_All();
}
/* ========Current Sensors Subroutines======== */
float getVPP1(int pin)
{
  float result1;
  int readValue1;             //value read from the sensor
  int maxValue1 = 0;          // store max value here
  int minValue1 = 1024;          // store min value here
  
   uint32_t start_time1 = millis();
   
   while((millis()-start_time1) < 200) //sample for 200 milliSeconds
   {
       readValue1 = analogRead(pin);
       if (readValue1 > maxValue1) 
       {
           maxValue1 = readValue1;
       }
       if (readValue1 < minValue1) 
       {
           minValue1 = readValue1;
       }
   }
   result1 = ((maxValue1 - minValue1) * 5.0)/1024.0;
   return result1;
  }


/*=====Lamp Current Sensing=====*/
void calcLamp()
{
   Voltage1 = getVPP1(ACS_lamp);
   VRMS1 = (Voltage1/2.0) *0.707; 
   LampAmp = (VRMS1 * 1000) / mVperAmp; 
   Lamp_mAmp = LampAmp * 1000;
   LampPow = (LampAmp * 220) * PF;
   last_time1 = current_time1;
   current_time1 = millis();    
   LampKWh = LampKWh +  LampPow *(( current_time1 - last_time1) /3600000.0) ; // calculating energy in Watt-Hour
   Lamp_Wh = LampKWh * 1000;
   // LampBill = LampKWh * (energyTariff/1000);
   // LampBill = LampKWh * energyTariff;
   if (LampKWh <= 100)
   {
      Lampcost = LampKWh * energyTariff100;
      LampBill = Lampcost;
   }   
   if (LampKWh > 100 && LampKWh <= 200)
   {
      NewLampBill = (LampKWh - 100) * energyTariff200;
      LampBill = Lampcost + NewLampBill;
   }
}

/*=====Fan Current Sensing=====*/
void calcFan()
{  
   Voltage2 = getVPP1(ACS_Fan);
   VRMS2 = (Voltage2/2.0) *0.707; 
   FanAmp = (VRMS2 * 1000) / mVperAmp;  
   Fan_mAmp = FanAmp * 1000;
   FanPow = (FanAmp * 9) * PF;
   last_time2 = current_time2;
   current_time2 = millis();    
   FanKWh = FanKWh +  FanPow *((current_time2 - last_time2) /3600000.0) ; // calculating energy in Watt-Hour   
   Fan_Wh = FanKWh * 1000;
   // FanBill = FanKWh * (energyTariff/1000);
   if (FanKWh <= 100)
   {
      Fancost = FanKWh * energyTariff100;
      FanBill = Fancost;
   }   
   if (FanKWh > 100 && FanKWh <= 200)
   {
      NewFanBill = (FanKWh - 100) * energyTariff200;
      FanBill = Fancost + NewFanBill;
   }
 }

/* ========Display All Data Subroutines======== */
void Disp_All()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("People: ");
  lcd.print(counter);
  lcd.setCursor(0,1);
  lcd.print("Temp.: ");
  lcd.print(t);
  lcd.print("*C");
  lcd.setCursor(0,2);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0,3);
  lcd.print("Tot. Cost: ");
  lcd.print(Total_Cost);
  lcd.print("Rs");
}

/* ========Display Sensor's Data Subroutines======== */
void Disp_Lamp()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("People: ");
    lcd.print(counter);  
    lcd.setCursor(0,1);  
    if (islightOn == 0)
    {
        lcd.print("Light: OFF "); 
    } 
    if (islightOn == 1)
    {
        lcd.print("Light: ON ");
    }
    lcd.setCursor(0,2);
    lcd.print(Lamp_mAmp);   
    lcd.print("mAmp");
    lcd.setCursor(9,2);
    lcd.print(LampPow);
    lcd.print("W");
    lcd.setCursor(0,3);
    lcd.print(Fan_Wh);
    lcd.print("Wh");
    lcd.setCursor(9,3);
    lcd.print(LampBill);
    lcd.print("Rs");
}

void Disp_Fan()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp.: ");
    lcd.print(t);
    lcd.print(" *C"); 
    lcd.setCursor(0,1);
    if (isFanOn == 0)
    {
        lcd.print("Fan: OFF "); 
    } 
    if (isFanOn == 1)
    {
        lcd.print("Fan: ON ");
    } 
    lcd.setCursor(0,2); 
    lcd.print(Fan_mAmp);
    lcd.print("mAmp");  
    lcd.setCursor(9,2);
    lcd.print(FanPow);
    lcd.print("W");
    lcd.setCursor(0,3);
    lcd.print(Fan_Wh);
    lcd.print("Wh");
    lcd.setCursor(9,3);
    lcd.print(FanBill);
    lcd.print("Rs");
}
