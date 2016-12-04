#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
// #include <MPU6050_6Axis_MotionApps20.h>
#include <TimerOne.h>  //
#include <SR04.h>
// #include <compass.h>


#include "line.h"
#include "sensor.h"
#include "debug.h"
#include "motor.h"
void flame_setup();
void xunxian_setup();
void compass_setup();


/********************************** 初始化 *********************************************/
void sensor_setup(){
        flame_setup();    //复眼
        xunxian_setup();    //下面的检测黑线红外循迹
        xunji_setup();      //前后循迹
        pinMode(tonePin,OUTPUT);
        pinMode(headswitchPin1,INPUT_PULLUP);   //前后触碰开关
        pinMode(headswitchPin2,INPUT_PULLUP);   
        compass_setup();    //罗盘设置
        huidu_setup();

}

/********************************MPU6050******************************************/

/********************************蜂鸣器*******************************************/
void Tone(int hz,int delay_time,int count){
        for(int i =0; i <count; i++) {

                for(int i=0; i<delay_time; i++)//输出一个频率的声音
                {
                        digitalWrite(tonePin,HIGH);//发声音
                        delayMicroseconds(hz);//延时1ms
                        digitalWrite(tonePin,LOW);//不发声音
                        delayMicroseconds(hz);//延时ms
                }

                delay(2*delay_time);
        }
}


/***********************************磁罗盘*********************************************/
bool Calibrate_flag;

void compass_setup(){
  char sTring[8];                      //用开关调试是否校准
  _seriaL_compass.begin(9600);
  pinMode(switchPin,INPUT_PULLUP);
  bool flag;
  {flag = digitalRead(switchPin);
  delay(5);
  if(flag == digitalRead(switchPin))
    flag = digitalRead(switchPin);
  else {
    delay(5);
    flag = digitalRead(switchPin);
  }} //读开关值并消抖 摁下flag为false 松开为true

  if(!flag){
    _seriaL_compass.write(0xC0);
    // _seriaL.println("Start calibrating.");
    Tone(500,50,2);
    while(!flag){flag = digitalRead(switchPin);
      delay(5);
      if(flag == digitalRead(switchPin))
        flag = digitalRead(switchPin);
      else {
        delay(5);
        flag = digitalRead(switchPin);
      } //读开关值并消抖 摁下flag为false 松开为true
    }
    _seriaL_compass.write(0xC1);
    // _seriaL_compass.readBytes(*sTring,8);
  _seriaL_compass.readBytes(sTring,8);
  _seriaL.println(sTring[6]);
    Tone(500,40,(sTring[6]-0x30));
  }
  else if(flag){
    _seriaL.println("Igore Calibrate");
    Tone(400,30,4);
  }
}


float headingdegrees(){
  // Serial.println("------------------------------------");
  char readings[8];
  _seriaL_compass.write(0x31);
  _seriaL_compass.readBytes(readings, 8);
  // for(int i=2;i<8;i++){
  //   Serial.print(readings[i]);
  // }
  //     Serial.println(' ');
  float result2=0;
  result2=100*(readings[2]-'0')+10*(readings[3]-'0')+(readings[4]-'0')+0.1*(readings[6]-'0');
  // Serial.println(result2);
  //Serial.print(result, DEC);
  ScreenData[0] = result2;
  return result2;
}


/*********************************************超声波***********************************/


SR04 sr04_f=SR04(echo1Pin,trig1Pin);
SR04 sr04_b=SR04(echo2Pin,trig2Pin);
extern int Ultrasound_f(){ //forward 前超声波
        ScreenData[1] = sr04_f.Distance();
        return ScreenData[1];
}
extern int Ultrasound_b(){ //backward 后超声波
        ScreenData[2] = sr04_b.Distance();
        return ScreenData[2];
}


/******************************************火焰传感器***********************************/
//初始化火焰传感器
void flame_setup(){
        pinMode(flame_a_1,INPUT);
        pinMode(flame_a_2,INPUT);
        pinMode(flame_a_3,INPUT);
        pinMode(flame_a_4,INPUT);
        pinMode(flame_a_5,INPUT);
        pinMode(flame_b_1,INPUT);
        pinMode(flame_b_2,INPUT);
        pinMode(flame_b_3,INPUT);
        pinMode(flame_b_4,INPUT);
        pinMode(flame_b_5,INPUT);
        pinMode(flame_c_1,OUTPUT);
        pinMode(flame_c_2,OUTPUT);
        pinMode(flame_c_3,OUTPUT);
        pinMode(flame_d_1,OUTPUT);
        pinMode(flame_d_2,OUTPUT);
        pinMode(flame_d_3,OUTPUT);
        pinMode(flame_c_out,INPUT);
        pinMode(flame_d_out,INPUT);
}

/***********************************************巡线************************************/
//初始化巡线
void xunxian_setup(){
        pinMode(xunxian1,INPUT);
        pinMode(xunxian2,INPUT);
        //pinMode(xunxian3,INPUT); //弃用
        pinMode(xunxian4,INPUT);
}

void xunji_setup(){
        pinMode(xunji0,INPUT);
        pinMode(xunji1,INPUT);
        pinMode(xunji2,INPUT);
        pinMode(xunji3,INPUT);
        pinMode(xunji4,INPUT);
        pinMode(xunji5,INPUT);
        attachInterrupt(digitalPinToInterrupt(xunji0),xunji0_change,CHANGE);
        attachInterrupt(digitalPinToInterrupt(xunji1),xunji1_change,CHANGE);
        attachInterrupt(digitalPinToInterrupt(xunji2),xunji2_change,CHANGE);
        attachInterrupt(digitalPinToInterrupt(xunji3),xunji3_change,CHANGE);

}

int xunji_flag[4]={0};  //Global value

void xunji0_change(){
	xunji_flag[0]=1;
}

void xunji1_change(){
    xunji_flag[1]=1;
}

void xunji2_change(){
    xunji_flag[2]=1;
}

void xunji3_change(){
    xunji_flag[3]=1;
}

bool xunxian_change(){    //在线上并在色块上为true 否则为false
  bool state;
  state = (digitalRead(xunxian1)||digitalRead(xunxian4))||digitalRead(xunxian2);
  delay(5);
  if(state == (digitalRead(xunxian1)||digitalRead(xunxian4))||digitalRead(xunxian2))
    return state;
  else{
    delay(5);
    state = (digitalRead(xunxian1)||digitalRead(xunxian4))||digitalRead(xunxian2);
    return state;
  }
}
/************** 灰度 *****************/
void huidu_setup(){
  pinMode(huidu_f, INPUT);
  pinMode(huidu_b, INPUT);
}

bool huidu_f_state(){   //return 1 while on color
  int state=analogRead(huidu_f);
  if(env_val==__left){
    if(state>_huidu_red)return 1;
    else return 0;
  }
  else if(env_val==__right){
    if(state>_huidu_blue)return 1;
    else return 0;
  }
}

bool huidu_b_state(){   //return 1 while on color
  int state=analogRead(huidu_b);
  if(env_val==__left){
    if(state>_huidu_red)return 1;
    else return 0;
  }
  else if(env_val==__right){
    if(state>_huidu_blue)return 1;
    else return 0;
  }
}