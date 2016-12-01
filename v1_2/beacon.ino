#define _front 0
#define _back 1
#define _left 1
#define _right 2
#define _near 0
#define _far 1
#define _notturn 0
#define _turn 1
#include <Arduino.h>
#include "line.h"
#include "sensor.h"
#include "env_val.h"

#define turn_speed 225
#define RUN_SPEED 225
#define motorBeginWork //驱动电机转动与否

int turn_counter = 0;
float beginDegree;
//  float HeadingInit; 【全局变量 定义在v1.1.ino】
bool find_if_beacon_on();
void turn_shun(int pwm);
void turn_ni(int pwm);
void turn_to_degree(float);
float offset_headingdegrees();
void turn_to_absolute_degree(float target_degree);

void step1_1(float degree){  // 走向并触碰灯塔，并返回到黑线，找正 degree为找正转角度
        // int d = Ultrasound_f();

        int flame_c;
        // while(1) {if(d > 8) {         //超生探测不到返回0 剔除0
        //                   motor_l_work(152);
        //                   motor_r_work(152);
        //                   d = Ultrasound_f();
        //           }
        //           else{
        //                   motor_stop();
        //                   delay(50);
        //                   for(int i = 150; i> 90; ) {
        //                           motor_l_work(i);
        //                           motor_r_work(i);
        //                           i -= 5;

        //                           delay(3);
        //                   }
        //                   motor_stop();
        //                   delay(50);
        //                   //_seriaL.println("tick");
        //                   break;
        //           }}}

        motor_l_work(110);
         motor_r_work(110);
         long int start_time=millis(),run_time;
         while(digitalRead(headswitchPin2)&&digitalRead(headswitchPin1)) {//Not touched 1 ;not closed 0
            run_time=millis()-start_time;  //长时间未触碰，判定卡住，跳出
            if(run_time>4000){
            break;
            }
            if(find_if_beacon_closed()&& run_time>3000){
              break;
            }
         }
         delay(100);
         motor_stop();


         motor_l_work(-220);
         motor_r_work(-220);

        motor_l_work(-150);
        motor_r_work(-150);

        while(!xunxian_change()) {}
        motor_stop();
       // if(find_if_beacon_closed())turn_counter++;
        turn_to_absolute_degree(180);
        turn_counter++;


}

void step1(){ //找到灯塔并转向并归中找正
        int flame_c[7],flame_d[7],flame_c_sum,flame_d_sum;
        // float headDegrees = headingdegrees();
        flame_c_sum = 0; flame_d_sum = 0;
        for(int i=2; i<7; i++) {   //去掉最边上两个红外接收器
                int a,b,c;
                a=i>>2; b=(i^(a<<2))>>1; c=i%2;
                digitalWrite(flame_c_3,!a);
                digitalWrite(flame_c_2,!b);
                digitalWrite(flame_c_1,!c);
                digitalWrite(flame_d_3,!a);
                digitalWrite(flame_d_2,!b);
                digitalWrite(flame_d_1,!c);
                flame_c[i-1] = analogRead(flame_c_out);
                flame_d[i-1] = analogRead(flame_d_out);
                flame_c_sum += flame_c[i-1];
                flame_d_sum += flame_d[i-1];
        }
        float i = flame_c_sum - flame_d_sum;
        float j = analogRead(flame_b_4) - analogRead(flame_b_2);   //右侧复眼前后
        float k = analogRead(flame_a_2) - analogRead(flame_a_4);   //左侧复眼前后

        #ifdef Game_pos_left
        float irlsum = __a_l * analogRead(flame_a_2) + __b_l*analogRead(flame_a_3) + __c_l * analogRead(flame_a_4);
        float irrsum = __d_l * analogRead(flame_b_2) + __e_l*analogRead(flame_b_3) + __f_l * analogRead(flame_b_4);
        #endif

        #ifdef Game_pos_right
        float irlsum = __a_r * analogRead(flame_a_2) + __b_r*analogRead(flame_a_3) + __c_r * analogRead(flame_a_4);
        float irrsum = __d_r * analogRead(flame_b_2) + __e_r*analogRead(flame_b_3) + __f_r * analogRead(flame_b_4);
        #endif

        ScreenData[5] = irlsum;
        ScreenData[6] = irrsum;

        int flag_near = 0;   //判断是否为近场
        int flag_qianhou = 0;   //判断灯塔在前面还是后面 0前 1后
        int flag_zuoyou = 0;   //判断灯塔左右 左1 右2
        int flag_turn = 0;   //判断是否该转 0不该 1该
        // _seriaL.print("i:");
        // _seriaL.print(i);
        // _seriaL.print("\t");
        //  _seriaL.print("irrsum: ");
        //  _seriaL.print(irrsum);
        // _seriaL.print("\t");
        //  _seriaL.print("irlsum: ");
        //  _seriaL.print(irlsum);
        // _seriaL.println("\t");
        // _seriaL.print("\t");
        // _seriaL.print(j);
        // _seriaL.print("\t");
        // _seriaL.println(k);
        // _seriaL.println(irlsum);
        // //_seriaL.println()


        // //_seriaL.println(analogRead(flame_a_4));
/******************判断电机状态****************************/

        if ((irrsum > 570 || irlsum > 570)) {   //用前后复眼判断太大是在附近 太小离太远//判断是否近场
                flag_near = _near;
                if (irrsum > irlsum) {   //判断左右
                        flag_zuoyou = _right;

                #ifdef Game_pos_left
                        if (irrsum > _left_r_sum) { //判断正对与否
                #endif

                #ifdef Game_pos_right
                        if (irrsum > _right_r_sum) {
                #endif
                                flag_turn = _turn;
                                // //_seriaL.println("turn flag updated");
                        }
                        else {
                                flag_turn = _notturn;
                                if ( j > 15)
                                        flag_qianhou = _front;
                                else if ( j < -15)
                                        flag_qianhou = _back;
                                // else
                                        // flag_turn = _turn;
                                // //_seriaL.println("turn flag not updated");
                        }
                }
                else if (irrsum < irlsum) {
                        flag_zuoyou = _left;
                        // //_seriaL.println("Left");
                #ifdef Game_pos_left
                        if (irlsum > _left_l_sum) {
                #endif

                #ifdef Game_pos_right
                        if (irlsum > _right_l_sum) {
                #endif

                                flag_turn = _turn;
                                // //_seriaL.println("turn flag updated");
                        }
                        else {
                                flag_turn = _notturn;

                                if ( k < -15)
                                        flag_qianhou = _back;
                                else if ( k > 15)
                                       flag_qianhou = _front;
                                else
                                        flag_turn = _turn;
                                // //_seriaL.println("turn flag not updated");
                        }
                }
        }
        else if ( i > 0) {   //判断灯塔前后
                flag_near = _far;
                flag_qianhou = _back;  //灯在后面
        }
        else if ( i < 0) {
                flag_near = _far;
                flag_qianhou = _front;  //灯在前面
        }

/******************使用flag驱动电机****************************/
  #ifdef motorBeginWork
        if((flag_turn == _turn) &&(flag_zuoyou== _right)) {
          if(flag_qianhou == _front){
          motor_l_work(-165);
          motor_r_work(-165);
          delay(150);
          motor_stop();}
          if(flag_qianhou == _back){
          motor_l_work(165);
          motor_r_work(165);
          delay(150);
          motor_stop();}
                // //_seriaL.println("prepare to trun right");
                //turn_to_degree(90);
                turn_to_absolute_degree(270);
                // //_seriaL.println("done");
                step1_1(180);
                motor_stop();
                delay(300);


        }
        else if((flag_turn == _turn) &&(flag_zuoyou== _left)) {
                if(flag_qianhou == _front){
                motor_l_work(-165);
                motor_r_work(-165);
                delay(150);
                motor_stop();}
                if(flag_qianhou == _back){
                motor_l_work(165);
                motor_r_work(165);
                delay(130);
                motor_stop();}
                // //_seriaL.println("prepare to trun left");
                //turn_to_degree(-90);
                turn_to_absolute_degree(90);
                // //_seriaL.println("done");
                step1_1(180);
                motor_stop();
                delay(300);
                // //_seriaL.println("trun left");
        }
        else if(flag_turn == _notturn) {
                if(flag_qianhou == _front) {
                        motor_l_work(RUN_SPEED);
                        motor_r_work(RUN_SPEED);
                        // //_seriaL.println("go forward");
                }
                else if(flag_qianhou == _back) {
                        motor_l_work(-RUN_SPEED);
                        motor_r_work(-RUN_SPEED);
                        // //_seriaL.println("go backward");
                }
        }
  #endif
}

void step2(){
	/*****/
		turn_to_absolute_degree(180);
	/******/
        motor_l_work(180);
        motor_r_work(180);
        //delay(100);
        int a=0;
        for(int i=0;i<5;i++){
          a+=Ultrasound_f();
        }
        a=a/5;
        while(a > 20) {
          for(int i=0;i<5;i++){
            a+=Ultrasound_f();
          }
          a=a/5;
          if(!(digitalRead(headswitchPin2)&&digitalRead(headswitchPin1)))break;
        }
        motor_stop();
}
int beacon(){
        // turn_to_absolute_degree(180);// HeadingInit=headingdegrees();
        // beginDegree = headingdegrees();

        int old_counter;
        do {
          step1();
        } while(turn_counter <4);
        old_counter = turn_counter;
        delay(500);
        while(find_if_beacon_on()){
           long int begin_time=millis();
           do{
             step1();
             if(millis()-begin_time>6000)break;
           }while(old_counter ==turn_counter);
           old_counter = turn_counter;
           delay(200);
        }
        step2();
        // turn_to_absolute_degree(180);

}

/*void turn_to_degree(float target_degree){     //转过指定角度 正为顺时针 负为逆时针 角度范围－360到＋360
        float begin_degree = headingdegrees(); //读取开始时候角度
        float now_degree =begin_degree;
        float err;
        err = target_degree - now_degree;
        if(begin_degree < (360 - target_degree) && begin_degree >= target_degree) {
                //_seriaL.println("1.........................");

                if(target_degree > 0)
                {while (abs(now_degree - begin_degree) < (target_degree - 2)) {
                         turn_shun(turn_speed);
                         now_degree = headingdegrees();
                         //_seriaL.println(now_degree - begin_degree);
                         motor_stop();
                 }}
                else if(target_degree < 0)
                {while (abs(now_degree - begin_degree) < (abs(target_degree) - 2)) {
                         turn_ni(turn_speed);
                         now_degree = headingdegrees();
                         //_seriaL.println(now_degree - begin_degree);
                         motor_stop();


                 }
                 //_seriaL.println("2......................"); }
        }
        else if(begin_degree >=0 && begin_degree < target_degree) {
                //_seriaL.println("1_1.....................");
                if(target_degree < 0 &&(now_degree >0&&now_degree < target_degree))
                        while (abs(now_degree - begin_degree) > (abs(target_degree) - 2)) {
                                turn_ni(turn_speed);
                                now_degree = headingdegrees();
                                //_seriaL.println(now_degree - begin_degree+360);
                                motor_stop();
                        }
                else if(target_degree < 0 &&(now_degree >360 - target_degree&&now_degree<360))
                        while (abs(now_degree - begin_degree+360) > (abs(target_degree) - 2)) {
                                turn_ni(turn_speed);
                                now_degree = headingdegrees();
                                //_seriaL.println(now_degree - begin_degree+360);
                                motor_stop();
                        }
                else{
                        //_seriaL.println("Wrong when judge headingdegrees");
                        delay(400);
                }
        }
        else if((begin_degree >=(360 - target_degree)) && (begin_degree<= 360)) {
                //_seriaL.println("1_1.....................");

                if(target_degree > 0 &&(now_degree >0&&now_degree<360 - target_degree))
                        while (abs(now_degree - begin_degree+360) > (target_degree - 2)) {
                                turn_shun(turn_speed);
                                now_degree = headingdegrees();
                                //_seriaL.println(now_degree - begin_degree+360);
                                motor_stop();
                        }
                else if(target_degree > 0 &&(now_degree >360 - target_degree&&now_degree <360))
                        while (abs(now_degree - begin_degree) > (target_degree - 2)) {
                                turn_shun(turn_speed);
                                now_degree = headingdegrees();
                                //_seriaL.println(now_degree - begin_degree+360);
                                motor_stop();
                        }
                else{
                        //_seriaL.println("Wrong when judge headingdegrees");
                        delay(400);
                }
        }
   }*/
void turn_to_degree(float target_degree){     //转过指定角度 正为顺时针 负为逆时针 【角度范围+-180以内】
        float begin_degree = headingdegrees(); //读取开始时候角度
        float now_degree =begin_degree;
        float err;
        err = target_degree - now_degree;
        if((begin_degree + target_degree)<360 && (begin_degree+target_degree)>=0) {
                //_seriaL.println("1..................");
                if(target_degree>0) {
                        while(abs(now_degree-begin_degree)<(target_degree-2)) {
                                turn_shun(turn_speed);
                                now_degree=headingdegrees();
                                //_seriaL.println(now_degree-begin_degree);
                                motor_stop();
                        }
                }
                if(target_degree<0) {
                        while(abs(now_degree-begin_degree)<(abs(target_degree)-2)) {
                                turn_ni(turn_speed);
                                now_degree=headingdegrees();
                                //_seriaL.println(now_degree-begin_degree);
                                motor_stop();
                        }
                }
                //_seriaL.println("1 end .........");
        }
        else if((begin_degree + target_degree)>=360 ) {  //  360度越界
                //_seriaL.println("2..................");
                if(target_degree>=0) {
                        while(abs(now_degree-begin_degree)<(target_degree-2)) {
                                turn_shun(turn_speed);
                                now_degree=headingdegrees();
                                //_seriaL.print("Now_degree  ");
                                //_seriaL.println(now_degree);
                                if(now_degree<180) now_degree+=360;
                                //_seriaL.println(now_degree-begin_degree);
                                motor_stop();
                        }
                }
                if(target_degree<0) {
                        //_seriaL.println("Judeg Angle Error 2");
                }
                //_seriaL.println("2 end................");
        }
        else if((begin_degree+target_degree)<0) {  // 0 度越界
                //_seriaL.println("3..................");
                if(target_degree>0) {
                        //_seriaL.println("Judeg Angle Error 3");
                }
                if(target_degree<=0) {
                        //_seriaL.println(abs(now_degree-360-begin_degree));
                        //_seriaL.println((abs(target_degree)-2));
                        while(abs(now_degree-begin_degree)<(abs(target_degree)-2)) {
                                turn_ni(turn_speed);
                                now_degree=headingdegrees();
                                if(now_degree>180) now_degree-=360;
                                //_seriaL.println(now_degree-begin_degree);
                                motor_stop();
                        }
                }
                //_seriaL.println("3 end.................");
        }

}


void turn_shun(int pwm){
        motor_l_work(pwm);
        motor_r_work(-pwm);
}
void turn_ni(int pwm){
        motor_l_work(-pwm);
        motor_r_work(pwm);
}

bool find_if_beacon_off_2(){  //检测场上是否还有灯塔亮,没有灯亮true 有灯亮false
        unsigned int flame_c_sum,flame_d_sum;
        long int flame_c[7],flame_d[7];
        for(int j = 1; j<=2; j++) {
                for(int i=0; i<=6; i++) { //去掉最边上两个红外接收器
                        int a,b,c;
                        a=i>>2; b=(i^(a<<2))>>1; c=i%2;
                        digitalWrite(flame_c_3,!a);
                        digitalWrite(flame_c_2,!b);
                        digitalWrite(flame_c_1,!c);
                        digitalWrite(flame_d_3,!a);
                        digitalWrite(flame_d_2,!b);
                        digitalWrite(flame_d_1,!c);
                        flame_c[i-1] = analogRead(flame_c_out);
                        flame_d[i-1] = analogRead(flame_d_out);
                        // _seriaL.println(flame_c[i-1]);
                        // _seriaL.println(flame_d[i-1]);

                        flame_c_sum += flame_c[i-1];
                        flame_d_sum += flame_d[i-1];
                }
                _seriaL.println(flame_c_sum+flame_d_sum);

        }


        //_seriaL.println((flame_c_sum + flame_d_sum)/10);
        return true;
}
/*
void turn_to_absolute_degree(float target_degree){ // 转到绝对坐标系中特定角度
        float begin_degree = headingdegrees();
        float err = target_degree - begin_degree;
        float now_err;

        if(err > 0&&err < 180) {
                while(now_err>6){
                        turn_shun(200);
                now_err = abs(headingdegrees()-target_degree);
                // _seriaL.println(now_err);
              }

        }
        else if(err < 360&&err >180) {
                while(now_err>6){
                        turn_ni(200);

                now_err = abs(headingdegrees()-target_degree);
                // _seriaL.println(now_err);
              }
        }
        else if(err <0&& err >-180) {
                while(now_err>6){
                        turn_ni(200);

                now_err = abs(headingdegrees()-target_degree);
                // _seriaL.println(now_err);
              }
        }
        else if(err < -180&& err > -360) {
                while(now_err>6){
                        turn_shun(200);
                now_err = abs(headingdegrees()-target_degree);
                // _seriaL.println(now_err);
              }
        }
        motor_stop();
}
*/
/****  返回基于校准角度 HeadingInit 的偏置坐标  ****/
float offset_headingdegrees(){  //全局变量 HeadingInit  将坐标调整为以初始值指向为180度的读数范围
	float off_heading=headingdegrees()+(180-HeadingInit);
	if(off_heading>=360)off_heading-=360;
	else if(off_heading<0)off_heading+=360;
 // _seriaL.print("off_heading  ");
 // _seriaL.println(off_heading);
 ScreenData[3] = off_heading;
	return off_heading;
}
/****  转到偏置坐标系中的角度  *****/
void turn_to_absolute_degree(float target_degree){ // 转到偏置坐标系中特定角度--设定第二阶段初始角度值为180，顺+->359.9，逆-->0;
	float begin_degree=offset_headingdegrees();
	float err=target_degree - begin_degree;
	if(err>=0){
		while(abs(err)>5){
			_seriaL.println("Shun");
			turn_shun(turn_speed);
			err=target_degree-offset_headingdegrees() ;
			_seriaL.println(err);
		}
   motor_stop();
	}
	else if(err<0){
		while(abs(err)>5){
			_seriaL.println("Ni");
			turn_ni(turn_speed);
			err=target_degree-offset_headingdegrees();
			_seriaL.println(err);
		}
   motor_stop();
	}
}




bool find_if_beacon_on(){
  int f_l[11],f_r[11];
  int f_max,f_l_max,f_r_max;
  f_l_max = 0;f_r_max = 0;
  f_l[10] = 0;
  f_r[10] = 0;
  f_l[0]=analogRead(flame_a_1);
  f_l[1]=analogRead(flame_a_2);
  f_l[2]=analogRead(flame_a_3);
  f_l[3]=analogRead(flame_a_4);
  f_l[4]=analogRead(flame_a_5);
  f_r[0]=analogRead(flame_b_1);
  f_r[1]=analogRead(flame_b_2);
  f_r[2]=analogRead(flame_b_3);
  f_r[3]=analogRead(flame_b_4);
  f_r[4]=analogRead(flame_b_5);


  turn_to_absolute_degree(90);

  f_l[5]=analogRead(flame_a_1);
  f_l[6]=analogRead(flame_a_2);
  f_l[7]=analogRead(flame_a_3);
  f_l[8]=analogRead(flame_a_4);
  f_l[9]=analogRead(flame_a_5);
  f_r[5]=analogRead(flame_b_1);
  f_r[6]=analogRead(flame_b_2);
  f_r[7]=analogRead(flame_b_3);
  f_r[8]=analogRead(flame_b_4);
  f_r[9]=analogRead(flame_b_5);

for(int i = 0;i<10;i++){
  f_r_max =max(f_r[i],f_r_max);
  f_l_max =max(f_l[i],f_l_max);
}
f_max = max(f_r_max,f_l_max);
// _seriaL.println(f_max);
turn_to_absolute_degree(180);
_seriaL.println("Find if beacon On");
_seriaL.println(f_max);
if(f_max>150){return true;}
else {return false;}
}

bool find_if_beacon_closed(){  //检测灯塔有没有被按灭，前面复眼.按灭返回1
    int flame_c[7],flame_c_min=1024;
        for(int i=2; i<5; i++) {   //去掉最边上两个红外接收器
                int a,b,c;
                a=i>>2; b=(i^(a<<2))>>1; c=i%2;
                digitalWrite(flame_c_3,!a);
                digitalWrite(flame_c_2,!b);
                digitalWrite(flame_c_1,!c);
                digitalWrite(flame_d_3,!a);
                digitalWrite(flame_d_2,!b);
                digitalWrite(flame_d_1,!c);
                flame_c[i] = analogRead(flame_c_out);
                flame_c_min=min(flame_c[i],flame_c_min);
        }
        if (flame_c_min>800)return 1;
        else return 0;

}
