int redPin = 11;
int greenPin = 10;
int bluePin = 9;
String comdata="";
int i=0;
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
    comdata += char(Serial.read());
    delay(2);
  }
  if(comdata.length()>0)
  {
    if(comdata[2]=='R')
    {
      pwm_R=0;
      for(i=7;i<comdata.length()-2;i++)
      {
        pwm_R=pwm_R*10+int(comdata[i]-'0');
      }
      
      Serial.print("R=");
      Serial.println(pwm_R);
    }
    if(comdata[2]=='G')
    {
      pwm_G=0;
      for(i=9;i<comdata.length()-2;i++)
      {
        pwm_G=pwm_G*10+int(comdata[i]-'0');
      }
      Serial.print("G=");
      Serial.println(pwm_G);
    }
    if(comdata[2]=='B')
    {
      pwm_B=0;
      for(i=8;i<comdata.length()-2;i++)
      {
        pwm_B=pwm_B*10+int(comdata[i]-'0');
      }
      Serial.print("B=");
      Serial.println(pwm_B);
    }
    comdata="";
  }
  setColor(pwm_R,pwm_G,pwm_B);
}
 
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, 255-red);
  analogWrite(greenPin, 255-green);
  analogWrite(bluePin, 255-blue);  
}
