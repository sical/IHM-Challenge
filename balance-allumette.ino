/*
 * https://circuits4you.com
 * 2016 November 25
 * Load Cell HX711 Module Interface with Arduino to measure weight in Kgs
 Arduino 
 pin 
 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/
//#include "ESP8266WiFi.h" 
//#include <WiFiUdp.h>
#include "HX711.h"  //You must have this library in your arduino library folder

//=============================================================================================
//                         Wifi stuff
//=============================================================================================

//const char* ssid = "ihmchallenge";              //your WiFi Name
//const char* password = "IHMCHALLENGE";  //Your Wifi Password
//WiFiServer server(6006);
//WiFiUDP Udp;
//IPAddress ip(10,42,0,1);             //IP to send UDP packets to



//void sendUdpMessage(char* mess)
//{
//    Udp.beginPacket(ip, 6006);
//    Serial.println(mess);
//    Udp.write(mess);
//    Udp.endPacket();
//}

bool shoot = true;
int lightSensorR = A2;
int lightSensorL = A1;

int outputR = 7;
int outputL = 2;

int lightStateR = 0;
int lightStateL = 0;

//=============================================================================================
//                         Balance stuff
//=============================================================================================
#define DOUT  5
#define CLK  6
float curw = 0.0;
float oldw = 0.0;

float domw= 0.025;
long debug = false;
HX711 scale;
 
//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = -96650; //-106600 worked for my 40Kg max scale setup 
 
//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);  
  if(debug) {
    Serial.println("Press T to tare");
  }
  scale.begin(DOUT, CLK);
  scale.set_scale(-187650);  //Calibration Factor obtained from first sketch
  scale.tare();             //Reset the scale to 0  

//=============================================================================================

  pinMode(lightSensorR, INPUT);
  pinMode(lightSensorL, INPUT);

  pinMode(outputR, OUTPUT);
  pinMode(outputL, OUTPUT);

  pinMode(13,OUTPUT);
  
}
 
//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  oldw = curw;
  curw =  scale.get_units(),3;
  if (curw - oldw >= domw ) {  
    
     balance_ping();
  }

  
  if(debug) {
    Serial.print("Weight: ");
    Serial.print(scale.get_units(), 3);  //Up to 3 decimal points
    Serial.println(" kg"); //Change this to kg and re-adjust the calibration factor if you follow lbs
   
    if(Serial.available()) {
      char temp = Serial.read();
      if(temp == 't' || temp == 'T')
        scale.tare();  //Reset the scale to zero      
    }
  }

//=============================================================================================

    lightStateR = analogRead(lightSensorR);
  lightStateL = analogRead(lightSensorL);
  


  if (lightStateR > 200){
    digitalWrite(outputR, HIGH);
    Serial.print("p_RIGHT");
  }
  else {
    digitalWrite(outputR, LOW);
    Serial.print("r_RIGHT");
  }

  if (lightStateL > 200){
    digitalWrite(outputL, HIGH);
    Serial.print("p_LEFT");
  }
  else{
    digitalWrite(outputL, LOW);
    Serial.write("r_LEFT");
  }
}



void balance_ping (){
  Serial.println("shoot!");
  digitalWrite(13, HIGH);
  delay(30);
  digitalWrite(13, LOW);
 
  }

//=============================================================================================
//                         Direction stuff
//=============================================================================================
