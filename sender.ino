#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define JoyStick_X A2  
#define JoyStick_Y A3  

int value=0;
RF24 radio(4,5);
const byte address[6] ="00001";
void setup() { 
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int x,y,z;
  x=analogRead(JoyStick_X);  
  y=analogRead(JoyStick_Y);  
  z=x*10000+y;
  Serial.print("z:");  
  Serial.println(z);
  char text[25];
  itoa(z,text,10);
  Serial.print("text:");  
  Serial.println(text);
  radio.write(&text, sizeof(text));
  delay(500);
}
