#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
long val=0;
int i;
RF24 radio(40,41);
const byte address[6] ="00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    for(i=0,val=0;i<strlen(text);i++){
      val=val*10+(text[i]-'0');
    }
    int y=val%10000;
    int x=val/10000;
    Serial.print("X:");  
    Serial.print(x);   
    Serial.print(":Y:");     
    Serial.println(y);  
    delay(500);
  }
}
