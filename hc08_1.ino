char serialData;  

void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
  while(Serial.available())
  {
    serialData =  Serial.read();
      
    if ('f' == serialData) {    
      Serial.print(serialData);   
      Serial.println(": forWard Command.");
    }   
    else if('b' == serialData) {  
      Serial.print(serialData);   
      Serial.println(": backWard Command.");
    }   
    else if( 'l' == serialData){  
      Serial.print(serialData);  
      Serial.println(": leftTurn Command.");
    }  
    else if('r' == serialData){  
      Serial.print(serialData);  
      Serial.println(": rightTurn Command.");
    }  
    else{  
    }  
  }
} 
