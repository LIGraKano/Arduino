int redPin = 11;
int greenPin = 10;
int bluePin = 9;
 
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
}
 
void loop()
{
  setColor(255, 0, 0);  // 红色
  delay(1000);
  setColor(0, 255, 0);  // 绿色
  delay(1000);
  setColor(0, 0, 255);  // 蓝色
  delay(1000);
  setColor(100, 255, 0);  // 黄色
  delay(1000);  
  setColor(80, 0, 100);  // 紫色
  delay(1000);
  setColor(0x32,0xB1,0x6C);  // 浅绿色
  delay(1000);
  setColor(0x8B,0x45,0x13);  // 马蹄棕
  delay(1000);
}
 
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, 255-red);
  analogWrite(greenPin, 255-green);
  analogWrite(bluePin, 255-blue);  
}
