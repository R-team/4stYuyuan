

//=============================  #define ===================================
#define IGNORE_RUNPOWER 25
#define MAXPWM 255


// =======================  Function Declaration  ==========================

int remote_setup();			//Set up PS2_comtroler for the robot
int remote_work();			//read signal and control the car
bool remote_check();			//judge whether to use PS2_controler

void (* resetFunc) (void) = 0;
void PS2_initial_config();  ////PS2_Receiver Connection Configure
void servo_initial();		//Set initial Servo position
void rotate(int n,int pos_n); //make servo[n] rotate to pos_n
void putdown();				//put down the arm and open the hand
void rise();				//rise the arm
void pick();				//control the hand to pick
void drop();				//open the hand
void rise_2();        //rise the arm
