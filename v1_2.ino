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
}

/*****************loop()*****************/
void loop() {
    //_seriaL.println(find_if_beacon_closed());
// find_if_beacon_off_2();
  // float k = analogRead(flame_a_1) - analogRead(flame_a_5);   //左侧复眼前后
  // ScreenData[7] = k;
  // screen_data_display();
  // delay(50);
        // for(int i = 0;i < 4;i++){
        // delay(1000);

          // find_if_beacon_off_2();

        // xunxian_flag = 0;
        // delay(200);
        // delay(30);
        // motor_l_work(200);
        // motor_r_work(200);
        // int flaggg;
        // find_if_beacon_on();
        // _seriaL.println(flaggg);
        // delay(500);
        // flaggg = 0;
        // _seriaL.println(headingdegrees());
        // delay(20000);
        // motor_l_work(-200);
        // motor_r_work(200);
        // delay(20000);
        // }
        // turn_to_degree(-90);
        // delay(500);
        // // Tone(500,50,5);
        // // // // _seriaL.println("tick");
        // turn_to_degree(90);
        // // // // _seriaL.println("tock");
        // delay(500);
//         turn_to_absolute_degree(90);
//         delay(500);
//         turn_to_absolute_degree(180);
//         delay(500);
//         turn_to_absolute_degree(270);
//         delay(500);
//         turn_to_absolute_degree(180);
//         delay(500);
        // Tone(500,50,5);
        // find_if_beacon_off_2();
        // delay(80);

// _seriaL.println(digitalRead(headswitchPin2)&&digitalRead(headswitchPin1));



/*==================================
====================================
==============以下是正式代码==========
====================================
====================================*/
         remote_check();
         while(remote_flag){
           remote_work();
         }
         Tone(500,50,3);
         beacon();
         Tone(500,50,3);
        #ifdef Game_pos_left
          climbing_left();
        #endif

        #ifdef Game_pos_right
          climbing_right();
        #endif
        // Tone(400,30,8);
          drop();
        Tone(400,40,8);
        while(1){}


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
