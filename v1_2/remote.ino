/*================================

 =================================*/

#include <Arduino.h>
#include <PS2X_lib.h>
#include "line.h"
#include "Servo.h"

//#define pressures   true
#define pressures   false
#define rumble      true
//#define rumble      false


PS2X ps2x;           // create PS2 Controller Class4
Servo *servo[3]={new Servo(),new Servo(),new Servo()};//create the servo class

int pos[3]={28,90,80};
  int error = 0;
  byte type = 0;
  byte vibrate = 0;

// =======================  Function Defination  ==========================

int remote_setup(){
  Serial.begin(115200);
  servo_initial();
  delay(500);
  PS2_initial_config(); //PS2_Receiver Connection Configure
              //Arduino will be RESET,if PS2_receiver is not connected correctly!
  return 1;
}

 bool remote_check(){
  ps2x.read_gamepad(false, vibrate);
  Serial.println("Run remote_check()");
  delay(1000);
  if(ps2x.Button(PSB_SELECT)){
      Serial.println("Remote Control END!");
      remote_flag=0;
  }
}
int remote_work(){
  int run_power = 0;
  int run_offset = 0;
  int pwm_L=0;
  int pwm_R=0;
  int run_power_list[5]={0,0,0,0,0};
  int run_offset_list[5]={0,0,0,0,0};
  ps2x.read_gamepad(false, vibrate); //Necessary!!! //read controller and set large motor to spin at 'vibrate' speed
  if(!ps2x.Button(PSB_R1)) {
      motor_stop();
      /*motor_l_work(0);
      motor_r_work(0);*/
  }
  if( ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      /*  Version 1
      run_power=(128-ps2x.Analog(PSS_LY));  //MAX PWM 125
      run_offset=(128-ps2x.Analog(PSS_LX));
      pwm_L=run_power-run_offset;
      pwm_R=run_power+run_offset;
      */

      /* ======================  Try Fliter  ============================*/
      int power_noise_flag =0; //Record the "NOISE" num
      int offset_noise_flag=0; //Record the "NOISE" num
      int power_sum=0;          //To calculate average
      int offset_sum=0;         //To calculate average
      for(int i=0;i<5;i++){
      run_power_list[i]=(128-ps2x.Analog(PSS_LY));  //MAX PWM 125
      run_offset_list[i]=(128-ps2x.Analog(PSS_LX));
      power_sum+=run_power_list[i];
      offset_sum+=run_offset_list[i];
      if(abs(run_power_list[i])<IGNORE_RUNPOWER)power_noise_flag+=1;
      if(abs(run_offset_list[i])<IGNORE_RUNPOWER)offset_noise_flag+=1;
      }

      run_power=power_sum/5;
      run_offset=offset_sum/5;
      if(power_noise_flag>=4 ){
        run_power=0;
      }
      if(offset_noise_flag>=4 ){
        run_offset=0;
      }

      pwm_L=2*(run_power-run_offset);
      pwm_R=2*(run_power+run_offset);
      /*   Try 1 END*/

      if(abs(pwm_L)>MAXPWM){
        pwm_L=pwm_L/abs(pwm_L)*MAXPWM;
        }
      if(abs(pwm_R)>MAXPWM){
        pwm_R=pwm_R/abs(pwm_R)*MAXPWM;
        }
      if(pwm_L==0 && pwm_R==0)motor_stop();
      else{
      motor_l_work(pwm_L);
      motor_r_work(pwm_R);
      }
      Serial.print(pwm_L);
      Serial.print(" , ");
      Serial.println(pwm_R);
    }
    if(ps2x.ButtonPressed(PSB_TRIANGLE)&& ! ps2x.Button(PSB_L1))
      rise_2();
    if(ps2x.ButtonPressed(PSB_TRIANGLE)&& ps2x.Button(PSB_L1))
      { rise();
        delay(500); //ok
        drop();
        delay(300);
        putdown();
      }
    if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
      drop();
    if(ps2x.ButtonPressed(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
      putdown();
    if(ps2x.ButtonPressed(PSB_SQUARE))              //will be TRUE if button was JUST released
      pick();
    if(ps2x.Button(PSB_SELECT)){
      Serial.println("Remote Control END！");
      remote_flag=0;
  }
  delay(50);

}

// =======================  Lower Function Defination  ==========================

void PS2_initial_config(){
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType();
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
  case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
   }
  if(error == 1){ //skip loop if no controller found
    resetFunc();  //RESET Arduino！
  }
}

void servo_initial(){
  servo[0]->attach(servo0);
  servo[1]->attach(servo1);
//  servo[2]->attach(servo2);
  for(int i=0;i<2;i++){
       servo[i]->write(pos[i]);
  }
  Serial.println("Servo_Initial_done");
}

void rotate(int n,int pos_n){
  for(;pos[n]!=pos_n;){
    pos[n]+=(pos_n-pos[n])/abs((pos_n-pos[n]));
   servo[n]->write(pos[n]);
    Serial.print("Servo:");
    Serial.print(n);
    Serial.print(" , ");
    Serial.println(pos[n]);
//    delay(10);
  }
}
void putdown(){
  rotate(1,15);
  rotate(0,90);
  delay(100);
  rotate(1,90);
}
void rise(){
  rotate(1,15);
  rotate(0,3);
}

void rise_2(){
  rotate(1,15);
  rotate(0,28);
}

void pick(){
  rotate(1,15);
}
void drop(){
    rotate(1,90);
}
