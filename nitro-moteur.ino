#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;
unsigned long delayLong;
float calib;
bool nitro;
bool prevNitro;
int prevMotor = 0;
const int buzzer = D2; 

// WIFI settings
#include "ESP8266WiFi.h" 
#include <WiFiUdp.h>

const char* ssid = "ihmchallenge";              //your WiFi Name
const char* password = "IHMCHALLENGE";  //Your Wifi Password
WiFiServer server(6006);
WiFiUDP Udp;
IPAddress ip(10,42,0,1);             //IP to send UDP packets to

//
void sendUdpMessage(char* mess)
{
    Udp.beginPacket(ip, 6006);
    Serial.println(mess);
    Udp.write(mess);
    Udp.endPacket();
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {

  pinMode(buzzer, OUTPUT); 
  digitalWrite(buzzer, LOW);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
  }
      Serial.begin(9600);
    while(!Serial); // time to get serial running
    Serial.println(F("BME280 test"));

    unsigned status;
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 200;
    delayLong = 1000;
    delay(delayLong);

    Serial.println("-- Starting calibration Test (please wait 10 seconds)--");
    calib = 0;

    for(int i = 0; i < 10; i++) {
      calib = calib + (bme.readPressure() / 100.0F);
      delay(delayLong);
    }
    calib = calib / 10;
    Serial.println(calib);

    Serial.println();
    tone(buzzer, 1000);
    delay(300); 
    tone(buzzer, 2000);
    delay(300);
    tone(buzzer, 3000);
    delay(300);
    noTone(buzzer);
    
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
  prevNitro = nitro;
  nitro = (((bme.readPressure() / 100.0F)-calib) > 20);
              if(prevNitro != nitro) {
              if(nitro == true){
                sendUdpMessage("P_NITRO");
              }
              if(nitro == false){
                sendUdpMessage("R_NITRO");
              }
            }

  float v = analogRead(A0);

  if(v < 1 && prevMotor != -1){
    prevMotor = -1;
    sendUdpMessage("R_ACCELERATE");
    sendUdpMessage("P_BRAKE");
  } else if (v > 10 && prevMotor != 1){
    prevMotor = 1;
    sendUdpMessage("R_BRAKE");
    sendUdpMessage("P_ACCELERATE");
  } else if ( v >= 1 && v <= 10 && prevMotor != 0){
    prevMotor = 0;
    sendUdpMessage("R_ACCELERATE");
    sendUdpMessage("R_BRAKE");
  }

      delay(delayTime);
     }