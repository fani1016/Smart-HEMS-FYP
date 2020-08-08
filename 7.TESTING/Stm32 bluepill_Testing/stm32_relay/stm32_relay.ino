int counter = 0;

#define in PB14
#define out PB15
#define relay PB13

void setup() 
{
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(PC13, OUTPUT);
}

void loop() 
{      
  if(digitalRead(in))
  {
   counter++;
   delay(500);
  }
  if(digitalRead(out))
  {
    if (counter>0)
    {
      counter--;
    }
   delay(500);
  }  
  if (counter > 0)
  {
      digitalWrite(PC13, LOW);
      digitalWrite(relay, LOW);
      delay(1000);
  }
  
  if (counter == 0)
  {
      digitalWrite(PC13, HIGH);
      digitalWrite(relay, HIGH);
      delay(1000);
  }
}
