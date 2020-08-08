/*  Written By: Farhan Tariq Janjua
 *  B.Sc Electrical Engineering
 *  www.APCOMS.edu.pk
  ---------------------------------------------------
  -Set Lamp Energy Limit in Controlling Lamp Section.
  -Set Fan Energy Limit in Controlling Fan Section.
*/

#include "DHT.h"
#include "LiquidCrystal.h"
const int rs = PB11, en = PB10, d4 = PA4, d5 = PA3, d6 = PA2, d7 = PA1; //STM32 Pins to which LCD is connected
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Initialize the LCD

#define DHTPIN PA0  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define RELAYLight PB12
#define RELAYFan PB13
#define in PB14
#define out PB15

int counter=0;
int islightOn=0;
int isFanOn=0;
char btVal=NULL;
int t = 0;
int h = 0;

/* ========Current Circuit variables definition======== */

#define currentPin1 PA15  
#define currentPin2 PA12   

int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
float PF = 0.80;
float energyTariff = 10.0; // Energy cost in PKR per unit (kWh)
unsigned long last_time1 =0;
unsigned long current_time1 =0;
float Voltage1 = 0;
float VRMS1 = 0;
float LampAmp = 0;
float LampPow = 0;
float LampKWh =0;             // Energy in kWh
float LampBill = 0;
int Lamp_Wh = 0;
int Lamp_mAmp = 0;
float Total_Cost = 0;
unsigned long last_time2 =0;
unsigned long current_time2 =0;
float Voltage2 = 0;
float VRMS2 = 0;
float FanAmp = 0;
float FanPow = 0;
float FanKWh =0;             // Energy in kWh
float FanBill = 0;
int Fan_Wh = 0;
int Fan_mAmp = 0;

void setup()
{
  lcd.begin(16,4);
  analogWrite(PA8,920);
  Serial1.begin(9600);
  dht.begin();
  pinMode(DHTPIN, INPUT); 
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(currentPin1, INPUT);
  pinMode(currentPin2, INPUT);
  pinMode(RELAYLight, OUTPUT);
  pinMode(RELAYFan, OUTPUT); 
  lcd.print("===Smart HEMS===");
  delay(3000);
      
  attachInterrupt(digitalPinToInterrupt(PA12),IN,RISING); 
  attachInterrupt(digitalPinToInterrupt(PA15),OUT,RISING); 
  attachInterrupt(digitalPinToInterrupt(PA10),BTI,RISING); 
}

void loop()
{  
/*=====Temp. and Humidity Sensing=====*/
  t = dht.readTemperature();
  h = dht.readHumidity();

  if(Serial1.available()>0) {}
  
/*=====Bluetooth Receiving=====*/
  BTI();
  
/*=====Presence Sensing=====*/
  if(digitalRead(in))
  {
   IN();
   delay(350);
  }
  if(digitalRead(out))
  {
   OUT();
   delay(350);
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
      if (btVal == 'c' || (t >= 25 && btVal == NULL))
        {
        digitalWrite(RELAYFan, HIGH);
        isFanOn = 1;
      }
  }
  
  if (isFanOn == 1)
  {
      if (btVal == 'd' || (t < 25 && btVal == NULL))
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
  Serial1.print(t);  // #01
  Serial1.print("|");
  Serial1.print(h);  // #02
  Serial1.print("|");
  Serial1.print(counter);  // #03
  Serial1.print("|");
  Serial1.print(Total_Cost);  // #04
  Serial1.print("|");
  Serial1.print(Lamp_mAmp);  // #05
  Serial1.print("|");
  Serial1.print(Lamp_Wh);  // #06
  Serial1.print("|");
  Serial1.print(Fan_mAmp);  // #07
  Serial1.print("|");
  Serial1.print(Fan_Wh);  // #08   
  Serial1.print("|");
  Serial1.print(LampBill);  // #09   
  Serial1.print("|");  
  Serial1.print(FanBill);  // #10   
  Serial1.print("|");
  
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
  btVal = Serial1.read();
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
    Disp_All();
  }     
}
/* ========Current Sensors Subroutines======== */
float getVPP1(int pin)
{
  float result1;
  int readValue1;             //value read from the sensor
  int maxValue1 = 0;          // store max value here
  int minValue1 = 4096;          // store min value here
  
   uint32_t start_time1 = millis();
   
   while((millis()-start_time1) < 200) //sample for 200 milliSeconds
   {
       readValue1 = analogRead(pin);
       // see if you have a new maxValue
       if (readValue1 > maxValue1) 
       {
           /*record the maximum sensor value*/
           maxValue1 = readValue1;
       }
       if (readValue1 < minValue1) 
       {
           /*record the maximum sensor value*/
           minValue1 = readValue1;
       }
   }
      // Subtract min from max
   result1 = ((maxValue1 - minValue1) * 5.0)/4096.0;
      
   return result1;
  }


/*=====Lamp Current Sensing=====*/
void calcLamp()
{
   Voltage1 = getVPP1(currentPin1);
   VRMS1 = (Voltage1/2.0) *0.707; 
   LampAmp = (VRMS1 * 1000) / mVperAmp; 
   Lamp_mAmp = LampAmp * 1000;
   LampPow = (LampAmp * 220) * PF;
   last_time1 = current_time1;
   current_time1 = millis();    
   LampKWh = LampKWh +  LampPow *(( current_time1 - last_time1) /3600000.0) ; // calculating energy in Watt-Hour
   Lamp_Wh = LampKWh * 1000;
   // LampBill = LampKWh * (energyTariff/1000);
   LampBill = LampKWh * energyTariff;
}

/*=====Fan Current Sensing=====*/
void calcFan()
{ 
   Voltage2 = getVPP1(currentPin2);
   VRMS2 = (Voltage2/2.0) *0.707; 
   FanAmp = (VRMS2 * 1000) / mVperAmp;  
   Fan_mAmp = FanAmp * 1000;
   FanPow = (FanAmp * 9) * PF;
   last_time2 = current_time2;
   current_time2 = millis();    
   FanKWh = FanKWh +  FanPow *((current_time2 - last_time2) /3600000.0) ; // calculating energy in Watt-Hour   
   Fan_Wh = FanKWh * 1000;
   // FanBill = FanKWh * (energyTariff/1000);
   FanBill = FanKWh * energyTariff;
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
  lcd.setCursor(-4,2);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(-4,3);
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
    lcd.println(counter);  
    lcd.setCursor(0,1);  
    if (islightOn == 0)
    {
        lcd.print("Light: OFF "); 
    } 
    if (islightOn == 1)
    {
        lcd.print("Light: ON ");
    }
    lcd.setCursor(-4,2);
    lcd.print(Lamp_mAmp);   
    lcd.print("mAmp");
    lcd.setCursor(5,2);
    lcd.print(LampPow);
    lcd.print("W");
    lcd.setCursor(-4,3);
    lcd.print(Fan_Wh);
    lcd.print("Wh");
    lcd.setCursor(5,3);
    lcd.print(LampBill);
    lcd.print("Rs");
}

void Disp_Fan()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp.: ");
    lcd.print(t);
    lcd.println(" *C"); 
    lcd.setCursor(0,1);
    if (isFanOn == 0)
    {
        lcd.print("Fan: OFF "); 
    } 
    if (isFanOn == 1)
    {
        lcd.print("Fan: ON ");
    } 
    lcd.setCursor(-4,2); 
    lcd.print(Fan_mAmp);
    lcd.print("mAmp");  
    lcd.setCursor(5,2);
    lcd.print(FanPow);
    lcd.print("W");
    lcd.setCursor(-4,3);
    lcd.print(Fan_Wh);
    lcd.print("Wh");    
    lcd.setCursor(5,3);
    lcd.print(FanBill);
    lcd.print("Rs");
}
