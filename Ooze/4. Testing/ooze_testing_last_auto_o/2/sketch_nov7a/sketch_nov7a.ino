#include "Print.h"
#include <Arduino.h>
#include <SensorModbusMaster.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SoftwareSerial.h>

const byte requestframe[] = {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC5, 0xCB};       //assigning request frame values
const byte requestframe2[] = {0x0A, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC4, 0xB3};

SoftwareSerial mod(32,33); // rx,tx
char buf0[4], buf1[4], buf2[4];
float Flow, dataT1, dataT2 ,Tot = 0.0;
char buf3[4], buf4[4], buf5[4];
float Flow1, dataT3, dataT4 ,Tot1 = 0.0;
int hydro_result = 0;
byte myHex1[18];
byte values1[18];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9500);
  mod.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("inside modbus2");
  memset(buf3,0,sizeof(buf3));                                                            //clearing the buffers
  memset(buf4,0,sizeof(buf4));
  memset(buf5,0,sizeof(buf5));
  if (mod.write(requestframe2, sizeof(requestframe2)) == 8)                                //first request frame will be given as input to flowmeter 
  {
    for (byte i = 0; i <18; i++) 
    {                                                                                    //response from flowmeter will be read here
      values1[i] = mod.read();
      myHex1[i] = values1[i];
      delay(10);
    }

    //buf3[0] = myHex1[7]; buf3[1] = myHex1[6]; buf3[2] = myHex1[5]; buf3[3] = myHex1[4];         //datas from flowmeter will be stored in buffer 
    uint32_t myLongNum = (myHex1[4] << 24) | (myHex1[5] << 16) | (myHex1[6] << 8) | (myHex1[7]);
    union{
      uint32_t intValue;
      float floatValue;
    }data;
    data.intValue = myLongNum;
    Flow1 = data.floatValue;
    Serial.print("myLongNum......");
    Serial.println(myLongNum);
    Serial.print("data.floatValue...");
    Serial.println(data.floatValue);
    Serial.println("Flow Rate = "+String(Flow1));
    Serial.print(values1[1], HEX);                                                         
    Serial.print(' ');
    Serial.print(values1[2], HEX);
    Serial.print(' ');
    Serial.print(values1[3], HEX);
    Serial.print(' ');
    Serial.print(values1[4], HEX);
    Serial.print(' ');
    Serial.print(values1[5], HEX);
    Serial.print(' ');
    Serial.print(values1[6], HEX);
    Serial.print(' ');
    Serial.print(values1[7], HEX);
    Serial.print(' ');
    Serial.print(values1[8], HEX);
    Serial.print(' ');
    Serial.print(values1[9], HEX);
    Serial.print(' ');
    Serial.print(values1[10], HEX);
    Serial.print(' ');
    Serial.print(values1[11], HEX);
    Serial.print(' ');
    Serial.print(values1[12], HEX);
    Serial.print(' ');
    Serial.print(values1[13], HEX);
    Serial.print(' ');
    Serial.print(values1[14], HEX);
    Serial.print(' ');
    Serial.print(values1[15], HEX);
    Serial.print(' ');
    Serial.print(values1[16], HEX);
    Serial.print(' ');
    Serial.print(values1[17], HEX);
    Serial.println(' ');
  }
}
