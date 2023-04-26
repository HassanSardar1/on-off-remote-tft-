#include <Adafruit_GFX.h>   
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h> 
#include <SPI.h>  
#include "RF24.h"
#include <EEPROM.h>
RF24 myRadio (A5, 10);
byte addresses[][6] = {"0"};
struct package {
  char onoff[4] = ""; 
};

typedef struct package Package;
Package dataRecieve;
Package dataTransmit;
#define  CHERRY   0xF020
#define YP A3 
#define XM A2 
#define YM 9 
#define XP 8 
#define TS_MINX 100
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define REDBAR_MINX 60
#define GREENBAR_MINX 100
#define BLUEBAR_MINX 60
#define REDBAR_MINY 10
#define GREENBAR_MINY 60
#define BLUEBAR_MINY 160
#define GREENBAR_HEIGHT 200
#define GREENBAR_WIDTH 30
#define BAR_HEIGHT 150
#define BAR_WIDTH 20
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define BLACK   0x0000
int BLUE = tft.color565(50, 50, 255);
#define DARKBLUE 0x0010
#define VIOLET 0x8888
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY   tft.color565(64, 64, 64);
#define GOLD 0xFEA0
#define BROWN 0xA145
#define SILVER 0xC618
#define LIME 0x07E0
int currentpcolour;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
int currentpage;
int redval;
int greenval;
int blueval;
int redpos = REDBAR_MINY + 12;
int greenpos = GREENBAR_MINY + 12;
int bluepos = BLUEBAR_MINY + 12;
int oldrpos = redpos;
int oldgpos = greenpos;
int oldbpos = bluepos;
int x, y;
boolean AutoManual = 0;
boolean pushButton1 = 0; //Declared 'A' button
boolean pushButton2 = 0; //Declared 'B' button
boolean pushButton3 = 0; //Declared 'D' button
boolean pushButton4 = 0; //Declared 'C' button       
int SwitchState = EEPROM.read(1);
int lastMySwitchState  =  EEPROM.read(2);
int button = 0 ;
void setup()
{
  Serial.begin(9600);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  tft.reset();
  tft.begin(tft.readID());
  Serial.println(); 
  Serial.print("reading id...");
  delay(500);
  Serial.println(tft.readID(), HEX);
  tft.fillScreen(BLACK);
  tft.setRotation(1);

  currentpage = 0;

  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(50, 140);
  tft.print("Loading...");

  tft.setTextColor(tft.color565(255, 255, 0));
  tft.setCursor(30, 70);
  tft.print("Welcome");

  tft.setCursor(30, 100);
  tft.print("By: Hassan");
  for (int i = 0; i < 250; i++)
  {
    tft.fillRect(30 - 10, 180, i, 10, RED);
    delay(1);
  }
  tft.fillScreen(BLACK);
 drawHome();

}

void loop()
{
 if ( myRadio.available()) {
    while (myRadio.available()){
      myRadio.read( &dataRecieve, sizeof(dataRecieve));
    }  
      
  }
    delay(50);
  myRadio.stopListening();
  
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));
  bool success = myRadio.write(&dataTransmit, sizeof(dataTransmit));

  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
TSPoint p = ts.getPoint();   
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
if (currentpage == 0)
  {   
    
    String onoff = dataRecieve.onoff;
   
   
    if(onoff=="off"){
   tft.setTextColor(BLACK);
      tft.setCursor(180, 175);
       tft.setTextSize(2);
       tft.fillRoundRect(100, 165, 200, 30, 8, WHITE);
        tft.drawRoundRect(100, 165, 200, 30, 8, RED); 
       tft.print("OFF");}
 if(onoff=="on"){
   tft.setTextColor(BLACK);
      tft.setCursor(180, 175);
       tft.setTextSize(2);
       tft.fillRoundRect(100, 165, 200, 30, 8, WHITE);
        tft.drawRoundRect(100, 165, 200, 30, 8, RED); 
       tft.print("ON");}

if(myRadio.available() || success){
  //Serial.println("sent!");
      tft.setTextColor(BLACK);
     tft.setCursor(150, 215);
       tft.setTextSize(2);
       tft.fillRoundRect(100, 205, 200, 30, 8, WHITE);
  tft.drawRoundRect(100, 205, 200, 30, 8, RED); 
       tft.print("Connected");}else
       {
        //Serial.println("fail!");
      tft.setTextColor(BLACK);
     tft.setCursor(110, 215);
       tft.setTextSize(2);
       tft.fillRoundRect(100, 205, 200, 30, 8, WHITE);
  tft.drawRoundRect(100, 205, 200, 30, 8, RED); 
       tft.print("not connected");
       }
       
    
    if (p.x > 115 && p.x < 290 && p.y > 123 && p.y < 850)
      { 
      
        if (p.z > ts.pressureThreshhold) {
          EEPROM.update(2,1);
        //Serial.println("Relay is ON");
delay(70);
          tft.fillRoundRect(17, 80, 280, 50, 8, RED);  
  tft.drawRoundRect(17, 80, 280, 50, 8, WHITE);
   tft.setCursor(30, 100);
  tft.print("OFF Relay");
    tft.fillRoundRect(17, 10, 280, 50, 8, GREEN);
  tft.drawRoundRect(17, 10, 280, 50, 8, WHITE); 
  tft.setCursor(40, 30);
  tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(BLACK);

        tft.print("Relay is ON");
 delay(70);
        x = 0;
        y = 0;
        p.z = 0;
         
      } }

 if (p.x > 380 && p.x < 544  && p.y > 125 && p.y < 830)
      {if (p.z > ts.pressureThreshhold) {
         EEPROM.update(2,0);
//Serial.println("Relay is OFF");
        delay(70); 
 
  tft.fillRoundRect(17, 10, 280, 50, 8, RED);
  tft.drawRoundRect(17, 10, 280, 50, 8, WHITE); 
   tft.setCursor(40, 30);
  tft.print("ON Relay");
       tft.fillRoundRect(17, 80, 280, 50, 8, GREEN);  
  tft.drawRoundRect(17, 80, 280, 50, 8, WHITE);
        tft.setCursor(30, 100);
          tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(BLACK);
        tft.print("Relay is OFF");
       delay(70);
        x = 0;
        y = 0;
        p.z = 0;
    }
}
  }

  if (EEPROM.read(1) == 0 && EEPROM.read(2) == 0) {   
    EEPROM.update(1,1);
    }
  if (EEPROM.read(1) == 1 && EEPROM.read(2) == 1) {   
     EEPROM.update(1,0);
  }
   EEPROM.update(0, EEPROM.read(1));
   if(EEPROM.read(0)==1){ 
     tft.fillRoundRect(17, 10, 280, 50, 8, RED);
  tft.drawRoundRect(17, 10, 280, 50, 8, WHITE);
  tft.setCursor(40, 30);
  tft.print("ON Relay");

       tft.fillRoundRect(17, 80, 280, 50, 8, GREEN);  
  tft.drawRoundRect(17, 80, 280, 50, 8, WHITE);
        tft.setCursor(30, 100);
          tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(BLACK);
    tft.print("Relay is OFF");
    strcpy(dataTransmit.onoff, "off" );
    }else if(EEPROM.read(0)==0){
      
  tft.fillRoundRect(17, 80, 280, 50, 8, RED);  
  tft.drawRoundRect(17, 80, 280, 50, 8, WHITE);
  tft.setCursor(30, 100);
  tft.print("OFF Relay");
  tft.fillRoundRect(17, 10, 280, 50, 8, GREEN);
  tft.drawRoundRect(17, 10, 280, 50, 8, WHITE); 
  tft.setCursor(40, 30);
  tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(BLACK);
  tft.print("Relay is ON");
  strcpy( dataTransmit.onoff, "on" );
  }
  }
void drawHome()
{
 tft.fillScreen(BLACK);
 tft.drawRoundRect(0, 0, 319, 240, 8, WHITE);    
  tft.fillRoundRect(17, 80, 280, 50, 8, RED);  
  tft.drawRoundRect(17, 80, 280, 50, 8, WHITE);
  tft.fillRoundRect(17, 10, 280, 50, 8, RED);
  tft.drawRoundRect(17, 10, 280, 50, 8, WHITE); 
  tft.fillRoundRect(100, 165, 200, 30, 8, WHITE);
  tft.drawRoundRect(100, 165, 200, 30, 8, RED); 
  tft.fillRoundRect(100, 205, 200, 30, 8, WHITE);
  tft.drawRoundRect(100, 205, 200, 30, 8, RED); 

  /*x0：x coordinate of the starting point
 y0：y coordinate of the starting point
 w：the length of the square 
 h：the width of the square
 radius：the radius of the round corners
color：the color of the square */
  tft.setCursor(10, 20);
  tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(BLACK);
  tft.setCursor(40, 30);
  tft.print("ON Relay");
  tft.setCursor(30, 100);
  tft.print("OFF Relay");
  tft.setCursor(10, 60);
  tft.setTextColor(GREEN);
  tft.setCursor(10, 170);
  tft.print("Relay");
  tft.setCursor(10, 210);
  tft.print("Remote");
  
}
