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
        flame_setup();
        xunxian_setup();
        pinMode(tonePin,OUTPUT);
        pinMode(headswitchPin1,INPUT_PULLUP);
        pinMode(headswitchPin2,INPUT_PULLUP);
        compass_setup();
        encoder_setup();

}

/********************************MPU6050******************************************/
long int  encoder_count[4]={0};   //Global Value
void encoder_setup(){
        pinMode(ENCODER_LF_A, INPUT); //encoder_count[0] A相
        pinMode(ENCODER_LB_A, INPUT); //encoder_count[1]
        pinMode(ENCODER_RF_A, INPUT); //encoder_count[2]
        pinMode(ENCODER_RB_A, INPUT); //encoder_count[3]
        pinMode(ENCODER_LF_B, INPUT); //encoder_count[0] B相
        pinMode(ENCODER_LB_B, INPUT); //encoder_count[1]
        pinMode(ENCODER_RF_B, INPUT); //encoder_count[2]
        pinMode(ENCODER_RB_B, INPUT); //encoder_count[3]
        attachInterrupt(digitalPinToInterrupt(ENCODER_LF_A),encoderCount_lf,CHANGE);
        attachInterrupt(digitalPinToInterrupt(ENCODER_LB_A),encoderCount_lb,CHANGE);
        attachInterrupt(digitalPinToInterrupt(ENCODER_RF_A),encoderCount_rf,CHANGE);
        attachInterrupt(digitalPinToInterrupt(ENCODER_RB_A),encoderCount_rb,CHANGE);
}
//  Ratary Encoder count function;
void encoderCount_lf(){
        if(digitalRead(ENCODER_LF_A)!=digitalRead(ENCODER_LF_B))encoder_count[0]++;
        else encoder_count[0]--;
}
void encoderCount_lb(){
        if(digitalRead(ENCODER_LB_A)!=digitalRead(ENCODER_LB_B))encoder_count[1]++;
        else encoder_count[1]--;
}
void encoderCount_rf(){
        if(digitalRead(ENCODER_RF_A)==digitalRead(ENCODER_RF_B))encoder_count[2]++;
        else encoder_count[2]--;
}
void encoderCount_rb(){
        if(digitalRead(ENCODER_RB_A)==digitalRead(ENCODER_RB_B))encoder_count[3]++;
        else encoder_count[3]--;
}
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
/*void compass_setup(){ //用串口调试是否校准
  _seriaL_compass.begin(9600);
  static char str[10] = {0};
  delay(100);
  while (!_seriaL.available());
  _seriaL.println("Choose if calibrate,Y or N");
  _seriaL.readBytes(str, 1);
  Tone(500,30,1);
  if((*str == 'Y')||(*str == 'y')){
    float begin_time = millis();
    _seriaL_compass.write(0xC0);
    _seriaL.println("Start calibrating.");
    Tone(500,50,2);
    _seriaL_compass.readBytes(str, 8);
    _seriaL.println(str);
    *str = 0;
    while(millis() - begin_time< 60000){
    now_time = millis();}
    _seriaL_compass.write(0xC1);
    _seriaL_compass.readBytes(str, 8);
    _seriaL.println(str);
    Tone(500,50,3);
  }
  else if((*str == 'N')||(*str == 'n')){
    _seriaL_println("Igore Calibrate");
    Tone(500,70,2);
  }

} */
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
  _seriaL.readBytes(sTring,8);
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


/*
hcm5831l 弃用

void compass_setup(){
        Wire.begin();

        // compass_x_gainError = 0.88;
        // compass_y_gainError = 0.94;
        // compass_z_gainError = 0.91;
        // compass_x_offset = 61.02;
        // compass_y_offset = 53.95;
        // compass_z_offset = 431.74;
        compass_init(2);
        compass_debug = 0;

        if(run_counter = 0)
                Calibrate_flag = 1;
        else {
                Calibrate_flag = 0;
                EEPROM.get(eeaddress,compass_x_gainError);
                eeaddress += sizeof(float);
                EEPROM.get(eeaddress,compass_y_gainError);
                eeaddress += sizeof(float);
                EEPROM.get(eeaddress,compass_z_gainError);
                eeaddress += sizeof(float);
                EEPROM.get(eeaddress,compass_x_offset);
                eeaddress += sizeof(float);
                EEPROM.get(eeaddress,compass_y_offset);
                eeaddress += sizeof(float);
                EEPROM.get(eeaddress,compass_z_offset);
                eeaddress += sizeof(float);
                _seriaL.println("Calibrated,not calibrated again");
        }

        if(Calibrate_flag) {
                motor_r_work(-200);
                motor_l_work(200);
                _seriaL.println("Calibrating");
                compass_offset_calibration(3);
                motor_stop();
                EEPROM.put(eeaddress,compass_x_gainError);
                eeaddress += sizeof(float);
                EEPROM.put(eeaddress,compass_y_gainError);
                eeaddress += sizeof(float);
                EEPROM.put(eeaddress,compass_z_gainError);
                eeaddress += sizeof(float);
                EEPROM.put(eeaddress,compass_x_offset);
                eeaddress += sizeof(float);
                EEPROM.put(eeaddress,compass_y_offset);
                eeaddress += sizeof(float);
                EEPROM.put(eeaddress,compass_z_offset);
                eeaddress += sizeof(float);
                _seriaL.println("Calibration Done");
        }


}
float headingdegrees()
{


        // Serial.print("x = ");
        // Serial.println(compass_x_scalled);
        // Serial.print("y = ");
        // Serial.println(compass_y_scalled);
        // Serial.print("z = ");
        // Serial.println(compass_z_scalled);


        compass_heading();
        #ifdef Bearing_print
        Serial.print ("Heading angle = ");
        Serial.print (bearing);
        Serial.println(" Degree");
        #endif
        delay(50);
        return(bearing);
}

*/
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
//初始化巡线。。。
void xunxian_setup(){
        pinMode(xunxian1,INPUT);
        pinMode(xunxian2,INPUT);
        pinMode(xunxian3,INPUT);
        pinMode(xunxian4,INPUT);
        // attachInterrupt(digitalPinToInterrupt(xunxian3),xunxian_change,CHANGE);
        // attachInterrupt(digitalPinToInterrupt(xunxian1),xunxian_change,RISING);
}
// void xunxian_change(){
//         xunxian_flag = 1;
// }
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

/***********************************************颜色***************************************/
// Init TSC230 and setting Frequency.
//tsc230变量
int g_count = 0;      // count the frequecy
int g_array[3];       // store the RGB value
int g_flag = 0;       // filter of RGB queue
float g_SF[3];        // save the RGB Scale factor

void TSC_Init()
{
        pinMode(S0, OUTPUT);
        pinMode(S1, OUTPUT);
        pinMode(S2, OUTPUT);
        pinMode(S3, OUTPUT);
        pinMode(OUT, INPUT);

        digitalWrite(S0, LOW); // OUTPUT FREQUENCY SCALING 2%
        digitalWrite(S1, HIGH);
}

// Select the filter color
void TSC_FilterColor(int Level01, int Level02)
{
        if(Level01 != 0)
                Level01 = HIGH;

        if(Level02 != 0)
                Level02 = HIGH;

        digitalWrite(S2, Level01);
        digitalWrite(S3, Level02);
}


void TSC_Count()
{
        g_count++;
}


void TSC_Callback()
{
        switch(g_flag)
        {
        case 0:
                //Serial.println("->WB Start");
                TSC_WB(LOW, LOW);      //Filter without Red
                break;
        case 1:
                //Serial.print("->Frequency R=");
                //Serial.println(g_count);
                g_array[0] = g_count;
                TSC_WB(HIGH, HIGH);    //Filter without Green
                break;
        case 2:
                //Serial.print("->Frequency G=");
                //Serial.println(g_count);
                g_array[1] = g_count;
                TSC_WB(LOW, HIGH);     //Filter without Blue
                break;

        case 3:
                //Serial.print("->Frequency B=");
                //Serial.println(g_count);
                //Serial.println("->WB End");
                g_array[2] = g_count;
                TSC_WB(HIGH, LOW);     //Clear(no filter)
                break;
        default:
                g_count = 0;
                break;
        }
}

void TSC_WB(int Level0, int Level1)      //White Balance
{
        g_count = 0;
        g_flag++;
        TSC_FilterColor(Level0, Level1);
        Timer1.setPeriod(1000000);       // set 1s period
}

void tcs230_setup()
{

//TSC_Init();
        Timer1.initialize();     // defaulte is 1s
        Timer1.attachInterrupt(TSC_Callback);
        attachInterrupt(0, TSC_Count, RISING);

        delay(4000);

        for(int i=0; i<3; i++)
                //Serial.println(g_array);

//白平衡
                g_SF[0] = 255.0/ g_array[0]; //R Scale factor
        g_SF[1] = 255.0/ g_array[1]; //G Scale factor
        g_SF[2] = 255.0/ g_array[2]; //B Scale factor

// Serial.println(g_SF[0]);
//Serial.println(g_SF[1]);
// Serial.println(g_SF[2]);

}



//red 与 blue时输出1
int tcs230_return()
{
        g_flag = 0;
        delay(500);
        int r,g,b;
        r=g_array[0] * g_SF[0];
        g=g_array[1] * g_SF[1];
        b=g_array[2] * g_SF[2];
        if((r>=150 and g<=70 and b<=70)or(r<=70 and g<=70 and b>=150)) //数值范围视具体情况而定
                return 1;
        else
                return 0;
}

void tcs230_test()
{

        g_flag = 0;
        Serial.print("[");
        for(int i=0; i<2; i++) {
                Serial.print(int(g_array[i] * g_SF[i]));
                Serial.print(",");
        }

        Serial.print(int(g_array[2] * g_SF[2]));
        Serial.println("]");
        delay(500);
        //rgb测试部分

        Serial.println(tcs230_return());

}
