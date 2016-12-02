/*说明 该头文件为各个阶段的环境变量，有如下内容
1.决定整体比赛在左面或者右面的flag
2.决定屏幕显示该哪些内容
2.决定各个阶段电机是否运行的flag
3.决定各个阶段是否打印某些值
4.Beacon阶段各种变量
  1）各列灯塔的触发值
  2）irlsum irrsum三个参数的值
*/




/**************决定比赛是左面还是右面***********************/
#define Game_pos_left
//#define Game_pos_right

/*****************决定屏幕显示哪些内容*********************/
// #define ir_data
// #define degree_display
// #define Ultrasound_display
// #define offsetdegree_display


/***************决定各个阶段底盘是否运动********************/

/*****************决定各个阶段是否打印某些值****************/


#define print_speed //pid中速度


/******************Beacon各个阶段某些变量*****************/
#define gray_blue


#define _left_l_sum 1330     //左右灯塔阀值
#define _left_r_sum 1660
#define _right_l_sum 1320
#define _right_r_sum 1650
/*-----------------------------*/
#define __a_l 0.3   //irlsum的三个参数
#define __b_l 1.3
#define __c_l 0.3
#define __d_l 0.3   //irrsum的三个参数
#define __e_l 1.3
#define __f_l 0.3

#define __a_r 0.3
#define __b_r 1.3
#define __c_r 0.3
#define __d_r 0.3
#define __e_r 1.3
#define __f_r 0.3
