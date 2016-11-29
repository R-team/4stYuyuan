#define flame_c_1 24 //s0 最低位
#define flame_c_2 26//s1 中
#define flame_c_3 28 //s2 高
#define flame_c_out 66 //接在A12口
#define flame_d_1 25 //s0
#define flame_d_2 27 //s1
#define flame_d_3 29 //s2
#define flame_d_out 65 //A11吧
void setup() {
  // put your setup code here, to run once:
pinMode(flame_c_1,OUTPUT);
        pinMode(flame_c_2,OUTPUT);
        pinMode(flame_c_3,OUTPUT);
        pinMode(flame_d_1,OUTPUT);
        pinMode(flame_d_2,OUTPUT);
        pinMode(flame_d_3,OUTPUT);
        pinMode(flame_c_out,INPUT);
        pinMode(flame_d_out,INPUT);
}

void loop() {
  int flame_c[8],flame_d[8];
  
  // put your main code here, to run repeatedly:
for(int i=3; i<6; i++) {   //去掉最边上两个红外接收器
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
                Serial.print(flame_c[i-1]);
                Serial.print(",");
                Serial.print(
        }
}
