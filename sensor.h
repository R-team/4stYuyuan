
//=============================  #define ===================================
#define IGNORE_RUNPOWER 25
#define MAXPWM 255


// =======================  Function Declaration  ==========================
void sensor_setup();
void  xunxian_setup();
bool xunxian_change();
void TSC_WB(int Level0, int Level1);
int Ultrasound_f();
int Ultrasound_b();
float headingdegrees();
void Tone(int hz,int delay_time,int count); // 以hz的响声 delay_time的间 隔
                                             //隔响 count次
