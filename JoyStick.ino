#define JoyStick_X A0  
#define JoyStick_Y A1  
const int  JoyStick_Z = 5 ; 

void setup()

{  
  Serial.begin(9600);  
  pinMode(JoyStick_Z,INPUT);
}  
  
void loop()

 {  
  int x,y,z;  
  x=analogRead(JoyStick_X);  
  y=analogRead(JoyStick_Y);  
  z=digitalRead(JoyStick_Z);
  Serial.print("X:");  
  Serial.print(x);   
  Serial.print(":Y:");     
  Serial.print(y);  
    
  if(z == HIGH)

  {  
    Serial.println(":B:1");        
  }

  else

  {  
    Serial.println(":B:0");  
  }    
  delay(1000);  
} 
