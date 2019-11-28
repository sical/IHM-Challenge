#include "ESP8266WiFi.h" 
#include <WiFiUdp.h>

const char* ssid = "ihmchallenge";              //your WiFi Name
const char* password = "IHMCHALLENGE";  //Your Wifi Password
int ledPin = 16;                        // pin D0
WiFiServer server(6006);
WiFiUDP Udp;
IPAddress ip(10,42,0,1);             //IP to send UDP packets to

int leftPin = 0;
int rightPin = 0;

int shootPin = 0;

int PrevleftPin = 0;
int PrevrightPin = 0;

int buttonState = 1;
int lastButtonState = 1;

void sendUdpMessage(char* mess) {
    Udp.beginPacket(ip, 6006);
    Serial.println(mess);
    Udp.write(mess);
    Udp.endPacket();
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  
  pinMode(D4, INPUT);

  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  
  pinMode(D2, INPUT); //SHOOT
  
}


void loop() {
  // put your main code here, to run repeatedly:

  leftPin = digitalRead(D5);
  rightPin = digitalRead(D6);
  shootPin = digitalRead(D2);

  if (leftPin == 1 && PrevleftPin == 0){
    sendUdpMessage("P_LEFT");
  }

  if (leftPin == 0 && PrevleftPin == 1){
    sendUdpMessage("R_LEFT");
  }

  if (rightPin == 1 && PrevrightPin == 0){
    sendUdpMessage("P_RIGHT");
  }

  if (rightPin == 0 && PrevrightPin == 1){
    sendUdpMessage("R_RIGHT");
  }
  if(shootPin == 1)  {
    sendUdpMessage("P_FIRE");
  }else {
 sendUdpMessage("R_FIRE");
    
  }
  


  Serial.print(leftPin);
  Serial.print("-----");
  Serial.print(rightPin);
  Serial.print("-----");
  Serial.print(shootPin);
  Serial.print("\n");
  
  PrevleftPin = leftPin;
  PrevrightPin = rightPin;

  buttonState = digitalRead(D4);

  if (buttonState == 0){
    if (lastButtonState == 1){
      sendUdpMessage("P_SKIDDING");
      Serial.print("P_SKIDDING");
      Serial.print("\n");
    }
  }

  if (buttonState == 1){
    if (lastButtonState == 0){
      sendUdpMessage("R_SKIDDING");
      Serial.print("R_SKIDDING");
      Serial.print("\n");
    }
  }

  lastButtonState = buttonState;
}
