
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "env_val.h"


/*  ScreenData----------
   -0- Headingdegree();
   -1- Ultrasound_f();

*/
const unsigned char myBitmap [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xf8, 0x3f, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x0f, 0xf0, 0x07, 0xff, 0xff,
  0xff, 0xff, 0xcf, 0xf7, 0xef, 0xf3, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xfb, 0x9f, 0xf9, 0xff, 0xff,
  0xff, 0xff, 0x8c, 0x1d, 0x8e, 0x1d, 0xff, 0xff, 0xff, 0xff, 0x01, 0xcf, 0x00, 0xce, 0xff, 0xff,
  0xff, 0xff, 0x07, 0xf5, 0x87, 0xf4, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xf9, 0x9f, 0xf9, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff,
  0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf7, 0xff, 0xff,
  0xff, 0xff, 0xef, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xef, 0xff, 0xff,
  0xff, 0xff, 0xf7, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xcf, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};




#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
void screen_setup(){
       display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
       display.clearDisplay();
       display.setTextSize(3);
       display.setTextColor(WHITE);
       display.setCursor(9,35);
       display.println("R-TEAM");
       display.drawBitmap(0,0,myBitmap,64,32,WHITE);
       display.drawBitmap(64,0,myBitmap,64,32,WHITE);
       display.display();
       delay(1000);
       display.clearDisplay();
}

void frame_setup(){
 display.drawLine(80,64,80,0,WHITE);
       display.drawLine(0,7,80,7,WHITE);
       display.setTextSize(1);
       display.setTextColor(WHITE);
       display.setCursor(0,0);
       display.println("Data Space");
       display.setCursor(92,0);
       display.println("Debug");
}

void screen_data_display(){
       display.clearDisplay();
       frame_setup();
       display.setTextSize(1);
       display.setTextColor(WHITE);
       display.setCursor(0,10);
#ifdef degree_display
       display.print("Degree: ");
       display.println(ScreenData[0]);
#endif
#ifdef Ultrasound_display
       display.print("Ultra_f: ");
       display.println(ScreenData[1]);
       display.print("Ultra_b: ");
       display.println(ScreenData[2]);
#endif
#ifdef offsetdegree_display
       display.print("OfDegree=: ");
       display.println(ScreenData[3]);
#endif
#ifdef ir_data
       display.print("irLeft: ");
       display.println(ScreenData[5]);
       display.print("irRight: ");
       display.println(ScreenData[6]);
#endif
       display.print(ScreenData[7]);
       // display.setTextSize(2);
       // display.setTextColor(WHITE);
       // display.setCursor(83,27);
       // display.print(ScreenData[4]);
       display.display();
}
void screen_debug_display(){


}