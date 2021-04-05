//11线AB相编码器（coder），减速比30，二倍频（使用一个中断和一个数字引脚），一圈被细分成11*30&2=660
//轮子编号：左前1，右前2，左后3，右后4，对应coder1~4
//实测在当前单位下，速度的值在100左右已经比较吃力了
#include<MsTimer2.h>

#define track_a_l 13
#define track_a_m 14
#define track_a_r 15
#define track_b_l 3
#define track_b_m 4
#define track_b_r 5
#define coder_1_A 2        
#define coder_1_B 14
#define coder_2_A 18
#define coder_2_B 23
#define coder_3_A 19          //外部中断4，引脚19
#define coder_3_B 15
#define coder_4_A 21
#define coder_4_B 20
#define motor_1_1 3
#define motor_1_2 4
#define motor_2_1 7
#define motor_2_2 8
#define motor_3_1 5
#define motor_3_2 6
#define motor_4_1 44
#define motor_4_2 45
#define Grayscale_max 0
#define Grayscale_min 0
#define Grayscale_judge 0
#define hengxian 2650
const int INTERVAL = 50;  //定时器中断间隔
float kp = 3.7 , ki = 1, kd = 0.00;
float tkp=0.2,tki;
int vx,vy,va;
String str = "";

int track_left_num,track_right_num,track_mid_num;
int t_error,t_last_error;

int stage=0;
unsigned long delaytime;

char c;
int LED =0,inv;
struct wheel{              //e(k)和e(k-1)都放在结构体里，在计速里计算赋值
  volatile long count_now;
  volatile long count_last;
  int expected_speed;
  volatile int current_speed;
  int error;
  int last_error;
  int pwm;                 //pwm可以是负值（-255~255），不能直接模拟输出
};
struct wheel fl,fr,bl,br;


void setup(){ 
  //pinMode(13,OUTPUT);
  pinMode(motor_1_1,OUTPUT);
  pinMode(motor_1_2,OUTPUT);
  pinMode(motor_2_1,OUTPUT);
  pinMode(motor_2_2,OUTPUT);
  pinMode(motor_3_1,OUTPUT);
  pinMode(motor_3_2,OUTPUT);
  pinMode(motor_4_1,OUTPUT);
  pinMode(motor_4_2,OUTPUT);
  
  attachInterrupt(0,coder_1,CHANGE);//外部中断0，引脚2
  attachInterrupt(5,coder_2,CHANGE);
  attachInterrupt(4,coder_3,CHANGE);
  attachInterrupt(2,coder_4,CHANGE);
  MsTimer2::set(INTERVAL,total_speed_count);
  MsTimer2::start();
  
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {//15当前p下15可过弯 60速可开环完成
  if(stage==0){
    vy=30;
    if(track_judge_a()){
      delay(500);
      stage++;
    }
  }else if(stage==1){
    vy=15;
    va=track_pid();
    if(track_judge_a()){
      va = 0;
      delaytime=millis();
      while(millis()-delaytime<700){
        //delay without delay()
      }
      stage++;
    }
  }else if(stage==2){
    vx = -60;
    vy = 0;
    delaytime=millis();
    Serial.println(millis());
    while(millis()-delaytime<1400){
      //delay without delay()
    }
    Serial.println(millis());
    vx = 0;
    vy = -60;
    delaytime=millis();
    while(millis()-delaytime<3000){
      //delay without delay()
    }
    stage++;
  }else if(stage==3){
    vy=0;
  }
//  switch(stage)
//  {
//    case 0:
//      vy=60;
//      if(track_judge_a()){
//        delay(500);
//        stage++;
//      }
//      break;
//    case 1:
//      vy=60;
//      va=track_pid();
//      if(track_judge_a()){
//        delay(700);
//        stage++;
//      }
//      break;
//    case 2:
//      vx = -60;
//      vy = 0;
//      delay(800);
//      vx = 0;
//      if(0==track_judge_b()){
//        vy = -60;
//      }else{
//        delay(500);
//        stage++;
//      }
//      break;
//    case 3:
//      if(track_judge_b()){
//        va=0;
//        vy=-60;
//        delay(500);
//        stage++;
//      }else{
//        vy=-60;
//        va=track_pid_2();
//      }
//      break;
//    
//      
//  }
  if(Serial2.available()){
    str= Serial2.readStringUntil('Z');
    vx =  str[2];
    vy = str[3];
  }
//  Serial.print(analogRead(track_a_l)+analogRead(track_a_m)+analogRead(track_a_r));
//  Serial.print(',');
//  Serial.println(stage);
  
  
  
  
//  //串口接收速度测试
//  if(Serial.available()){
//    inv = 0;
//    while(Serial.available()){
//      c=Serial.read();
//      inv *= 10;
//      inv += c-'0';
//    }
//  }
//  Serial.print(br.current_speed);
//  Serial.print(",");
//  Serial.print(fr.current_speed);
//  Serial.print(",");
//  Serial.print(br.count_now);
//  Serial.print(",");
//  Serial.println(br.count_now);
////  Serial.print(bl.current_speed);
////  Serial.print(",");
////  Serial.print(br.current_speed);
////  Serial.print(",");
////  Serial.println(fr.current_speed);
//  delay(50);
}

void total_speed_count(){       //改变pwm值也应该在定时器中断里
  fl.expected_speed=vy+vx+va;
  fr.expected_speed=vy-vx-va;
  bl.expected_speed=vy-vx+va;
  br.expected_speed=vy+vx-va;
  speed_count(&fl);
  speed_count(&fr);
  speed_count(&bl);
  speed_count(&br);
  fl.pwm+=pid(fl);
  fr.pwm+=pid(fr);
  bl.pwm+=pid(bl);
  br.pwm+=pid(br);
  motor_run();
}

void speed_count(struct wheel* n){//计算速度，本次误差和上次误差 
  n->last_error=n->error;
  n->current_speed = n->count_now-n->count_last;
  n->error= n->current_speed-n->expected_speed;//计算error
  n->count_last = n->count_now;
//  LED = 1 - LED;
//  digitalWrite(13,LED);
}

int pid(struct wheel n){     //据说调速用PI就可以了
  float delta_pwm = 0.0;
  
  
  delta_pwm+=kp*(n.error-n.last_error);              //P
  
  delta_pwm+=ki*n.error;                             //I
  
  return int(delta_pwm);
}

int track_pid(){     //据说调速用PI就可以了
  t_error = get_track_error();
  float track_v = 0.0;
  
  track_v+=tkp*t_error;              //P
  
  track_v+=tki*t_error;                             //I

  //t_last_error = t_error;
  return int(track_v);
}


int track_pid_2(){
  t_error = get_track_error_2();
  float track_v = 0.0;
  
  track_v+=tkp*(t_error-t_last_error);              //P
  
  track_v+=tki*t_error;                             //I

  t_last_error = t_error;
  return int(track_v);
}



void motor_run(){//用一个模拟引脚和一个数字引脚或者两个模拟引脚？之后实验一下00和11的区别
//fl
  if(fl.pwm>=0){
    digitalWrite(motor_1_1,LOW);
    analogWrite(motor_1_2,fl.pwm);
  }else{
    digitalWrite(motor_1_2,LOW);
    analogWrite(motor_1_1,-fl.pwm);
  }
//bl
  if(bl.pwm>=0){
    digitalWrite(motor_3_1,LOW);
    analogWrite(motor_3_2,bl.pwm);
  }else{
    digitalWrite(motor_3_2,LOW);
    analogWrite(motor_3_1,-bl.pwm);
  }
//fr
  if(fr.pwm>=0){
    digitalWrite(motor_2_1,LOW);
    analogWrite(motor_2_2,fr.pwm);
  }else{
    digitalWrite(motor_2_2,LOW);
    analogWrite(motor_2_1,-fr.pwm);
  }
//br
  if(br.pwm>=0){
    digitalWrite(motor_4_1,LOW);
    analogWrite(motor_4_2,br.pwm);
  }else{
    digitalWrite(motor_4_2,LOW);
    analogWrite(motor_4_1,-br.pwm);
  }
}



void coder_1(){
  if(digitalRead(coder_1_A)){
    if(digitalRead(coder_1_B)){
      fl.count_now++;
    }else{
      fl.count_now--;
    }
  }else{
    if(digitalRead(coder_1_B)){
      fl.count_now--;
    }else{
      fl.count_now++;
    }
  }
}
void coder_2(){
  if(digitalRead(coder_2_A)){
    if(digitalRead(coder_2_B)){
      fr.count_now++;
    }else{
      fr.count_now--;
    }
  }else{
    if(digitalRead(coder_2_B)){
      fr.count_now--;
    }else{
      fr.count_now++;
    }
  }
}
void coder_3(){
  if(digitalRead(coder_3_A)){
    if(digitalRead(coder_3_B)){
      bl.count_now++;
    }else{
      bl.count_now--;
    }
  }else{
    if(digitalRead(coder_3_B)){
      bl.count_now--;
    }else{
      bl.count_now++;
    }
  }
}
void coder_4(){
  if(digitalRead(coder_4_A)){
    if(digitalRead(coder_4_B)){
      br.count_now++;
    }else{
      br.count_now--;
    }
  }else{
    if(digitalRead(coder_4_B)){
      br.count_now--;
    }else{
      br.count_now++;
    }
  }
}


//寻迹模块相关函数
void Serialout_track_analog(){
  //Serial.print("Value_analog:");
  Serial.print(analogRead(track_a_l));
  Serial.print(',');
  Serial.print(analogRead(track_a_m));
  Serial.print(',');
  Serial.print(analogRead(track_a_r));
  Serial.print(',');
  Serial.print(analogRead(track_b_l));
  Serial.print(',');
  Serial.print(analogRead(track_b_m));
  Serial.print(',');
  Serial.print(analogRead(track_b_r));
  Serial.println();
}
int get_track_error(){//x方向的偏移
  int error_a;
  track_left_num=analogRead(track_a_l);
  track_right_num=analogRead(track_a_r);
  track_mid_num=analogRead(track_a_m);
  if(track_mid_num>Grayscale_judge){
    error_a = track_left_num-track_right_num;
  }
  else{
    if(track_left_num>track_right_num){
      error_a = 2*Grayscale_max-(track_left_num-track_right_num);
    }
    else{
      error_a = 2*Grayscale_min-(track_left_num-track_right_num);
    }
  }
  return -(error_a);
}
int get_track_error_2(){//x方向的偏移
  int error_b;
  track_left_num=analogRead(track_b_l);
  track_right_num=analogRead(track_b_r);
  track_mid_num=analogRead(track_b_m);
  if(track_mid_num>Grayscale_judge){
    error_b = track_left_num-track_right_num;
  }
  else{
    if(track_left_num>track_right_num){
      error_b = 2*Grayscale_max-(track_left_num-track_right_num);
    }
    else{
      error_b = 2*Grayscale_min-(track_left_num-track_right_num);
    }
  }
  return error_b;
}
int track_judge_a(){
  int sum = analogRead(track_a_l)+analogRead(track_a_m)+analogRead(track_a_r);
  if(sum>hengxian){
    return 1;
  }
  return 0;
}
int track_judge_b(){
  int sum = analogRead(track_b_l)+analogRead(track_b_m)+analogRead(track_b_r);
  if(sum>hengxian){
    return 1;
  }
  return 0;
}
