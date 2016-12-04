/*********************************
应对实验室磁场畸变
左边坡上坡处转角改为80度，整体偏10度


********************************/
#define _speed 200
//#include <HMC5883L.h>
#include <Arduino.h>
#include <Wire.h>
#include "line.h"
#include "motor.h"
#include "sensor.h"
#include <compass.h>

void turn_to_degree();
void turn_to_absolute_degree();

void climbing_right()
{
      if(
        (digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){  //避免直接怼在斜坡上，无法测距
          motor_l_work(-120);
          motor_r_work(-120);
          delay(200);
          motor_stop();
        }

        turn_to_absolute_degree(90);
        Tone(300,20,1);

        float distanceActualF_b;
        while (1)
        {

                distanceActualF_b = Ultrasound_b();
                if (distanceActualF_b > 70)
                {
                        motor_l_work(-_speed);
                         motor_r_work(-_speed);
                        delay(10);
                }
                else if (distanceActualF_b < 66)
                {
                        motor_l_work(_speed);
                         motor_r_work(_speed);
                        delay(10);
                }
                else if (distanceActualF_b >= 66 && distanceActualF_b <= 70)
                        {motor_stop();
                        break;}

        }
        Tone(300,20,1);


       turn_to_absolute_degree(180);
        Tone(300,20,1);


        float distanceActualF_c;
        while (1)
        {
                distanceActualF_c = Ultrasound_f();
                if (distanceActualF_c > 9)
                {
                        motor_l_work(100);
                        motor_r_work(100);

                }
                else if (distanceActualF_c < 7)
                {
                        motor_l_work(-100);
                        motor_r_work(-100);

                }
                else if (distanceActualF_c >= 7 && distanceActualF_c < 9)
                       { motor_stop();
                        break;}
        }
        Tone(300,20,1);

        turn_to_absolute_degree(270);
        Tone(300,20,1);

        // 爬坡阶段
        motor_l_work(_speed);
        motor_r_work(_speed);
//        int a[10]={0,0,0,0,0,0,0,0,0,0};
//        for(int i=0;;i++)
//        {
//          while(1)
//          {
//            if(Ultrasound_f()<14)
//            break;
//          }
//          a[i]=1;
//          if(a[4]!=0)
//          break;
//        }
        while((digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){}
        while((digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){}
        while((digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){}
        motor_stop();
        motor_l_work(-150);
        motor_r_work(-150);
        delay(249);
        motor_stop();
        //倒车

//        float distanceActualF_d;
//        while (1)
//        {
//                distanceActualF_d = Ultrasound_f();
//                motor_l_work(-100);
//                motor_r_work(-100);
//                if (distanceActualF_d > 10)
//                {
//                        motor_stop();
//                        break;
//
//                }
//
//        }
//        Tone(400,30,15);
//        int averD[5]={20,20,20,20,20};
//        for(int i=0;;i++)
//        {
//                int j=i%5;
//                averD[j]=Ultrasound_f();
//                double averd=(averD[0]+averD[1]+averD[2]+averD[3]+averD[4])/5.0;
//                if(averd<14.0)
////                Tone(0.003000,1);
//                break;
//        }
//        motor_stop();
}
void climbing_left()
{
        if((digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){  //避免直接怼在斜坡上，无法测距
          motor_l_work(-150);
          motor_r_work(-150);
          delay(200);
          motor_stop();
        }
//        float headingDegrees = headingdegrees();


//如果在第三块区域上
        // while (1)
        // {
        //         float headingActual_1 = headingdegrees();
        //         float error_1 = headingActual_1 - heading_1;
        //         if (error_1 > 3)
        //         {
        //                 //
        //                 motor_l_work(_speed);
        //                 motor_r_work(_speed);
        //                 delay(10);
        //         }
        //         else if (error_1 < -3)
        //         {
        //                 //
        //                 motor_l_work(_speed);
        //                 motor_r_work(_speed);
        //                 delay(10);
        //         }
        //         else if (error_1 <= 3 && error_1 >= -3)
        //                 break;
        // }

//        float distanceActualF_a;
//        while (1)
//        {
//                distanceActualF_a = Ultrasound_f();
//                if(distanceActualF_a>20)
//                {
//                  Tone(300,30,3);
//                }
//                if (distanceActualF_a > 12)
//                {
//                        motor_l_work(120);
//                         motor_r_work(120);
//
//                }
//                else if (distanceActualF_a < 10)
//                {
//                        motor_l_work(-90);
//                         motor_r_work(-90);
//
//                }
//                else if (distanceActualF_a >= 10 && distanceActualF_a <= 12)
//                       { motor_stop();
//                        break;}
//        }
//        Tone(300,20,1);

        turn_to_absolute_degree(270);
        Tone(300,20,1);

        float distanceActualF_b;
        while (1)
        {

                distanceActualF_b = Ultrasound_b();
                if (distanceActualF_b > 70)
                {
                        motor_l_work(-_speed);
                         motor_r_work(-_speed);
                }
                else if (distanceActualF_b < 66)
                {
                        motor_l_work(_speed);
                         motor_r_work(_speed);
                }
                else if (distanceActualF_b >= 66 && distanceActualF_b <= 70)
                        {motor_stop();
                        break;}

        }
        Tone(300,20,1);


        turn_to_absolute_degree(180);
        Tone(300,20,1);


        float distanceActualF_c;
        while (1)
        {
                distanceActualF_c = Ultrasound_f();
                if (distanceActualF_c > 9)
                {
                        motor_l_work(100);
                        motor_r_work(100);
                }
                else if (distanceActualF_c < 7)
                {
                        motor_l_work(100);
                        motor_r_work(100);
                }
                else if (distanceActualF_c >= 7 && distanceActualF_c < 9)
                       { motor_stop();
                        break;}
        }
        Tone(300,20,1);

        turn_to_absolute_degree(90);///
        Tone(300,20,1);

        // 爬坡阶段
        motor_l_work(_speed);
        motor_r_work(_speed);
        int a[10]={0,0,0,0,0,0,0,0,0,0};
//        for(int i=0;;i++)
//        {
//          while(1)
//          {
////            if((!digitalRead(headswitchPin1))&&(!digitalRead(headswitchPin2)))
//            if(Ultrasound_f()<14)
//            break;
//          }
//          a[i]=1;
//          if(a[4]!=0)
//          break;
//        }
        while((digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){}
        while((digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){}
        while((digitalRead(headswitchPin1))&&(digitalRead(headswitchPin2))){}
        motor_stop();

        //后退
        motor_l_work(-150);
        motor_r_work(-150);
        delay(253);
        motor_stop();

}
