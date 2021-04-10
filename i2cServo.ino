#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  625 // this is the 'maximum' pulse length count (out of 4096)
#define MIN_PULSE_WIDTH  500
#define MAX_PULSE_WIDTH  2500
#define FREQUENCY 60
// our servo # counter
uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit");
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

int pulseWidth(int angle){ //This function calculates servo's motion angle.
int pulse_wide, analog_value;
pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH); //This function get angle from 0 to 180 degrees and map from length minimum value to maximum. 
analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
Serial.println(analog_value);
return analog_value; //The value this function returns.
}

void loop() {
  // Drive each servo one at a time
  Serial.println(servonum);
  pwm.setPWM(15, 0, pulseWidth(90));
//  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
//    pwm.setPWM(0, 0, pulselen);
//    pwm.setPWM(1, 0, pulselen);
//    pwm.setPWM(2, 0, pulselen);
//    pwm.setPWM(3, 0, pulselen);
//    pwm.setPWM(4, 0, pulselen);
//    pwm.setPWM(5, 0, pulselen);
//    pwm.setPWM(6, 0, pulselen);
//    pwm.setPWM(7, 0, pulselen);
//    pwm.setPWM(8, 0, pulselen);
//    pwm.setPWM(9, 0, pulselen);
//    pwm.setPWM(10, 0, pulselen);
//    pwm.setPWM(11, 0, pulselen);
//    pwm.setPWM(12, 0, pulselen);
//    pwm.setPWM(13, 0, pulselen);
//    pwm.setPWM(14, 0, pulselen);
//    pwm.setPWM(15, 0, pulselen);
//  }
  delay(500);
  pwm.setPWM(15, 0, pulseWidth(0));
//  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
//    pwm.setPWM(0, 0, pulselen);
//    pwm.setPWM(1, 0, pulselen);
//    pwm.setPWM(2, 0, pulselen);
//    pwm.setPWM(3, 0, pulselen);
//    pwm.setPWM(4, 0, pulselen);
//    pwm.setPWM(5, 0, pulselen);
//    pwm.setPWM(6, 0, pulselen);
//    pwm.setPWM(7, 0, pulselen);
//    pwm.setPWM(8, 0, pulselen);
//    pwm.setPWM(9, 0, pulselen);
//    pwm.setPWM(10, 0, pulselen);
//    pwm.setPWM(11, 0, pulselen);
//    pwm.setPWM(12, 0, pulselen);
//    pwm.setPWM(13, 0, pulselen);
//    pwm.setPWM(14, 0, pulselen);
//    pwm.setPWM(15, 0, pulselen);
//  }
  delay(500);
}
