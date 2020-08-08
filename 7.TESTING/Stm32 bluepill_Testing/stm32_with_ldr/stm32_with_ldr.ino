int LDR = PA0; 
  
void setup()
{ 
  Serial.begin(9600);                                       
  pinMode(PC13, OUTPUT);          
}
 
void loop()
{
    int val = analogRead(LDR);
    if (val > 550)
    {   
      Serial.print(val); 
      digitalWrite(PC13, LOW);
      Serial.println("    LED OFF");
    }
        else  
    {      
      Serial.print(val); 
      digitalWrite(PC13, HIGH); 
      Serial.println("    LED ON"); 
    }
  delay(1000);
}
