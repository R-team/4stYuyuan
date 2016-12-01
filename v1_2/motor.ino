#include <Arduino.h>

#include "line.h"
#include "PID_v1.h"
#include "env_val.h"
extern long int encoder_count[4];
int motor_pwm[4];
int setpoint[4];
float Kp[4],Kd[4],Ki[4];


long int pid_timer;



void motor_speed_cal(float setpoint[4]){
  if(millis() - pid_timer > 80){
    static float speed_err[4],speed_intergal[4],speed_last_err[4];
    for(int i = 0;i<4;i++){
      speed_err[i] = encoder_count[i] - setpoint[i];
      motor_pwm[i] = Kp[i]*speed_err[i] + Ki[i]*speed_intergal[i] +Kd[i]*(speed_err[i] - speed_last_err[i]);
      if(motor_pwm[i] > 255)motor_pwm[i] = 255;
      else if(motor_pwm[i] < -255)motor_pwm[i] = -255;
      speed_last_err[i] = speed_err[i];
    }

    #ifdef print_speed
    _seriaL.print(encoder_count[0]);
    _seriaL.print(",");
    _seriaL.print(encoder_count[1]);
    _seriaL.print(",");
    _seriaL.print(encoder_count[2]);
    _seriaL.print(",");
    _seriaL.println(encoder_count[3]);
    #endif

    for(int i = 0;i<4;i++){
      encoder_count[i] = 0;
    }
    pid_timer = millis();
  }
}

void motor_work_set(int motor_pwm[4]){
  motor_lf_work(motor_pwm[0]);
  motor_lb_work(motor_pwm[1]);
  motor_rf_work(motor_pwm[2]);
  motor_rb_work(motor_pwm[3]);
}

void motor_work(float setpoint[4]){
  motor_speed_cal(setpoint);
  motor_work_set(motor_pwm);
}



//初始化电机，放setup里
void motor_setup() {
  pinMode(motor_lf_in1, OUTPUT);
  pinMode(motor_lf_in2, OUTPUT);
  pinMode(motor_lf_en, OUTPUT);
  pinMode(motor_rf_in1, OUTPUT);
  pinMode(motor_rf_in2, OUTPUT);
  pinMode(motor_rf_en, OUTPUT);
  pinMode(motor_lb_in1, OUTPUT);
  pinMode(motor_lb_in2, OUTPUT);
  pinMode(motor_lb_en, OUTPUT);
  pinMode(motor_rb_in1, OUTPUT);
  pinMode(motor_rb_in2, OUTPUT);
  pinMode(motor_rb_en, OUTPUT);
}



//运行函数,pwm:-255~255,正值正转，负值反转
int motor_lf_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_lf_in1, HIGH);
    digitalWrite(motor_lf_in2, LOW);
    analogWrite(motor_lf_en, pwm);
  }
  else {
    digitalWrite(motor_lf_in1, LOW);
    digitalWrite(motor_lf_in2, HIGH);
    analogWrite(motor_lf_en, -pwm);
  }
}

int motor_rf_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_rf_in1, HIGH);
    digitalWrite(motor_rf_in2, LOW);
    analogWrite(motor_rf_en, pwm);
  }
  else {
    digitalWrite(motor_rf_in1, LOW);
    digitalWrite(motor_rf_in2, HIGH);
    analogWrite(motor_rf_en, -pwm);
  }
}

int motor_lb_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_lb_in1, HIGH);
    digitalWrite(motor_lb_in2, LOW);
    analogWrite(motor_lb_en, pwm);
  }
  else {
    digitalWrite(motor_lb_in1, LOW);
    digitalWrite(motor_lb_in2, HIGH);
    analogWrite(motor_lb_en, -pwm);
  }
}

int motor_rb_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_rb_in1, HIGH);
    digitalWrite(motor_rb_in2, LOW);
    analogWrite(motor_rb_en, pwm);
  }
  else {
    digitalWrite(motor_rb_in1, LOW);
    digitalWrite(motor_rb_in2, HIGH);
    analogWrite(motor_rb_en, -pwm);
  }
}

void motor_l_work(int pwm){
  motor_lb_work(pwm);
  motor_lf_work(pwm);
}

void motor_r_work(int pwm){
  motor_rb_work(pwm);
  motor_rf_work(pwm);
}

//刹车&锁死轮胎
void motor_stop(){
    digitalWrite(motor_lf_in1, HIGH);
    digitalWrite(motor_lf_in2, HIGH);
    analogWrite(motor_lf_en, 255);
    digitalWrite(motor_lb_in1, HIGH);
    digitalWrite(motor_lb_in2, HIGH);
    analogWrite(motor_lb_en, 255);

    digitalWrite(motor_rf_in1, HIGH);
    digitalWrite(motor_rf_in2, HIGH);
    analogWrite(motor_rf_en, 255);
    digitalWrite(motor_rb_in1, HIGH);
    digitalWrite(motor_rb_in2, HIGH);
    analogWrite(motor_rb_en, 255);


}


void motor_test() {
  motor_l_work(255);
  motor_r_work(255);
  delay(1000);
  motor_l_work(150);
  motor_r_work(150);
  delay(1000);
  motor_l_work(-255);
  motor_r_work(-255);
  delay(1000);
  motor_l_work(-150);
  motor_r_work(-150);
  delay(1000);
}
