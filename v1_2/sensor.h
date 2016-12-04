
//=============================  #define ===================================
#define IGNORE_RUNPOWER 25
#define MAXPWM 255


// =======================  Function Declaration  ==========================
void sensor_setup();
void  xunxian_setup();
void xunji_setup();
void huidu_setup();
void xunji0_change();
void xunji1_change();
void xunji2_change();
void xunji3_change();
bool xunxian_change();
void TSC_WB(int Level0, int Level1);
int Ultrasound_f();
int Ultrasound_b();
float headingdegrees();
void Tone(int hz,int delay_time,int count); // 以hz的响声 delay_time的间 隔
                                             //隔响 count次
bool huidu_f_state();
bool huidu_b_state();