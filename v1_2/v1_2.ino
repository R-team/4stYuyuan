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
float tmp; //调试变量

/***************** Setup()******************/
void setup() {
        _seriaL.begin(115200);
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
        motor_l_work(200);
        motor_r_work(-200);

}
extern long int encoder_count[4];
/*****************loop()*****************/
void loop() {
  //  putdown();
  // m_speed();
  _seriaL.println(encoder_count[2]);
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
        //  remote_check();
        //  while(remote_flag){
        //    remote_work();
        //  }
        // //  Tone(500,50,3);
        //  //beacon();
        // //  Tone(500,50,3);
        // #ifdef Game_pos_left
        //   climbing_left();
        // #endif
        //
        // #ifdef Game_pos_right
        //   climbing_right();
        // #endif
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
