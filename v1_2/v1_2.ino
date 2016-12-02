#include <Arduino.h>
#include "line.h"
#include "motor.h"
#include "remote.h"
#include "sensor.h"
#include "env_val.h"
// #include "HMC5883L.h"
#include <Wire.h>
#include <EEPROM.h>
#include "debug.h"

#define __left 1
#define __right 0
/*******************各类flag*************/
bool remote_flag=1;   //标志遥控状态 1为需要遥控，0为停止遥控
bool car_pos_flag;
bool xunxian_flag; //巡线flag 1为黑线 0为白色

/*********************全局变量声明*************/
long int run_counter; //程序运行次数计数器
int eeaddress;
float HeadingInit;
float ScreenData[9];

void turn_to_degree(float);
void turn_to_absolute_degree(float target_degree);
int beacon();
void climbing_right();
void climbing_left();
bool find_if_beacon_closed();

bool env_val;
/***************** Setup()******************/
void setup() {
        _seriaL.begin(115200);
        pinMode(envSwitch,INPUT_PULLUP);
        env_val = digitalRead(envSwitch);
        delay(2);
        while(env_val != digitalRead(envSwitch)){
          env_val = digitalRead(envSwitch);
          delay(2); //防抖
        }
        delay(2);
        while(env_val != digitalRead(envSwitch)){
          env_val = digitalRead(envSwitch);
          delay(2); //防抖
        }

        _seriaL.println(env_val);



        EEPROM.get(eeaddress,run_counter); //从EEPROM获取计数信息
        eeaddress += sizeof(long int);

        sensor_setup();

        motor_setup();
         remote_setup();
        // screen_setup();
        // _seriaL.println(run_counter);
        run_counter ++;
        EEPROM.put(0,run_counter); //初始化完成，运行计数器加一
        Tone(300,400,1);
        HeadingInit = headingdegrees();


}


float tmp[4];
extern long int encoder_count[4];
/*****************loop()*****************/
void loop() {
  //  putdown();

  // env_val = digitalRead(envSwitch);
  // delay(2);
  // while(env_val != digitalRead(envSwitch)){
  //   env_val = digitalRead(envSwitch);
  //   delay(2); //防抖
  // }
  // delay(2);
  // while(env_val != digitalRead(envSwitch)){
  //   env_val = digitalRead(envSwitch);
  //   delay(2); //防抖
  // }
  //
  // _seriaL.println(env_val);
  // m_speed();

  //  motor_l_work(150);
  //  motor_r_work(150);
  //  delay(3000);
  //  Tone(600, 300, 1);
   //
  //  turn_to_degree(90);
  //  Tone(500, 30, 4);
   //
  //  rise();
  //  Tone(400, 100, 2);
/*==================================
====================================
==============以下是正式代码==========
====================================
====================================*/
         remote_check();
         while(remote_flag){
           remote_work();
           motor_speed_cal(tmp);
         }
        //  Tone(500,50,3);
        //  beacon();
        //  Tone(500,50,3);
        //
        // if(env_val == __left)
        //   climbing_left();
        // else if(env_val == __right)
        //   climbing_right();
        //
        // // Tone(400,30,8);
        //   drop();
        // Tone(400,40,8);
        // while(1){}


}

void time_test(){
  long int begin_time = millis();
  long int err_time;
  Ultrasound_f();
  err_time = millis() - begin_time;
  _seriaL.print("Ultrasound time");
  _seriaL.print("\t");
  _seriaL.print(err_time);
  _seriaL.print("\t");
  begin_time = millis();
//  find_if_beacon_off_2();
  err_time = millis() - begin_time;
  _seriaL.print("BeaconDetect time");
  _seriaL.print("\t");
  _seriaL.println(err_time);
  delay(100);



}
