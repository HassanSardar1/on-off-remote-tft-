#include <EEPROM.h>
#include <Wire.h> 
#include <SPI.h>  
#include "RF24.h"
RF24 myRadio (8, 9);
byte addresses[][6] = {"0"};

struct package {
  char onoff[4] = "";
};

const int relayPin1 = 5; //Declared Relay1
const int relayPin2 = 4; //Declared Relay2
const int relayPin3 = 3; //Declared Relay3
const int relayPin4 = 2; //Declared Relay3

typedef struct package Package;
Package dataRecieve;
Package dataTransmit;

void setup() {
  Serial.begin(9600);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );

  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();

pinMode(relayPin1, OUTPUT);                 //Relay output for START
pinMode(relayPin2, OUTPUT);                //Relay output for STOP
pinMode(relayPin3, OUTPUT);               //Relay output for LIGHT
pinMode(relayPin4, OUTPUT);               //Relay output for LIGHT

checkfailure();
}


void loop() {





 if ( myRadio.available()) {
    while (myRadio.available()){
      myRadio.read( &dataRecieve, sizeof(dataRecieve) );
    } }
    delay(50);
   myRadio.stopListening();
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
    String onoff = dataRecieve.onoff;


     if(onoff=="on")
    {
      strcpy( dataTransmit.onoff, "on");
      EEPROM.update(0,0);
     digitalWrite(relayPin1, HIGH);         //SETTING HIGH
     digitalWrite(relayPin2, HIGH);        //SETTING HIGH
     digitalWrite(relayPin3, HIGH);       //SETTING HIGH
     digitalWrite(relayPin4, HIGH);       //SETTING HIGH
    }
    if (onoff=="off")
    {
      strcpy( dataTransmit.onoff, "off");
      EEPROM.update(0,1);
     digitalWrite(relayPin1, LOW);         //SETTING HIGH
     digitalWrite(relayPin2, LOW);        //SETTING HIGH
     digitalWrite(relayPin3, LOW);       //SETTING HIGH
     digitalWrite(relayPin4, LOW);       //SETTING HIGH
    } 

}
void checkfailure() {
  if(EEPROM.read(0)==1){
    strcpy( dataTransmit.onoff, "off");
  digitalWrite(relayPin1, LOW);         //SETTING HIGH
digitalWrite(relayPin2, LOW);        //SETTING HIGH
digitalWrite(relayPin3, LOW);       //SETTING HIGH
digitalWrite(relayPin4, LOW);       //SETTING HIGH
return;
  }else if (EEPROM.read(0)==0){
    strcpy( dataTransmit.onoff, "on");
digitalWrite(relayPin1, HIGH);         //SETTING HIGH
digitalWrite(relayPin2, HIGH);        //SETTING HIGH
digitalWrite(relayPin3, HIGH);       //SETTING HIGH
digitalWrite(relayPin4, HIGH);       //SETTING HIGH
return;
}
  
  }
