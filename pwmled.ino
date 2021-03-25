int redPin = 11;
int greenPin = 10;
int bluePin = 9;
char serialData;
int pwm_R=0,pwm_G=0,pwm_B=0;
void setup()
{
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
}
 
void loop()
{
  while(Serial.available())
  {
    serialData =  Serial.read();
    if (serialData=='R') {    
      setColor(255,0,0);
    }   
    else if(serialData=='G') {  
      setColor(0,255,0);
    }   
    else if(serialData=='B'){  
      setColor(0,0,255);
    }  
    else{  
    }  
  }
}
 
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, 255-red);
  analogWrite(greenPin, 255-green);
  analogWrite(bluePin, 255-blue);  
}
