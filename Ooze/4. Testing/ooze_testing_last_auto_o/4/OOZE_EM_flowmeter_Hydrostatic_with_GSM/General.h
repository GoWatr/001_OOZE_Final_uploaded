#include "Print.h"
#include <Arduino.h>
#include <SensorModbusMaster.h>
#include <ArduinoJson.h>
#include <GSMSimHTTP.h>
#include <GSMSimTime.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
LiquidCrystal_I2C lcd(0x3F,16,2); 

const byte requestframe[] = {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC5, 0xCB};       //assigning request frame values
const byte requestframe2[] = {0x0A, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC4, 0xB3};
//{0x0B, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0x62};

SoftwareSerial mod(32,33); // rx,tx
char buf0[4], buf1[4], buf2[4];
float Flow, dataT1, dataT2 ,Tot = 0.0;
char buf3[4], buf4[4], buf5[4];
float Flow1, dataT3, dataT4 ,Tot1 = 0.0;
int hydro_result = 0;
int value1, value2, value1_2 = 0;
byte myHex1[18];
byte values1[18];
byte myHex[10];
byte values[10];
int Raw, Reset, count = 0;
String sensorvalue = "";    //hydrostatic sensor
//int hydro_result;

 void lcd_init(){
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(2, 0);
    lcd.print("GOWATR  SWMS");
}

void lcd_data_sent(){
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("DATA  SENT");
}
void lcd_data_fail(){
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("DATA  FAIL");
}   
    
  
void MODBUS() 
{                                                                                         //modbus function to read the flowmeter value 
  memset(buf0,0,sizeof(buf0));                                                            //clearing the buffers
  if (mod.write(requestframe, sizeof(requestframe)) == 8)                                //first request frame will be given as input to flowmeter 
  {
    for (byte i = 1; i <=10; i++) 
    {                                                                                    //response from flowmeter will be read here
      values[i] = mod.read();
      myHex[i] = values[i];
      delay(10);
    }

    Serial.print("the value of 5 is ");
    Serial.println(values[5], HEX);
    Serial.print("the value of 6 is ");
    Serial.println(values[6], HEX);
    //sensorvalue = (String(values[5])+String(values[6]));
    //Serial.println(sensorvalue); 3DE = 944
    hydro_result = (values[5] << 8) | values[6]; //This line has to be implemented for concating. (Bitwise operation)
    
    Serial.print("The SOLVED RESULT is ");
    Serial.println(hydro_result);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" HS Sensor :");
    lcd.setCursor(12, 0);
    lcd.print(hydro_result);
    lcd.setCursor(0, 1);
    lcd.print("Water Level:");
    delay(2000);
    Serial.print(values[1], HEX);                                                         
    Serial.print(' ');
    Serial.print(values[2], HEX);
    Serial.print(' ');
    Serial.print(values[3], HEX);
    Serial.print(' ');
    Serial.print(values[4], HEX);
    Serial.print(' ');
    Serial.print(values[5], HEX);
    Serial.print(' ');
    Serial.print(values[6], HEX);
    Serial.print(' ');
    Serial.print(values[7], HEX);
    Serial.print(' ');
    Serial.print(values[8], HEX);
    Serial.print(' ');
    Serial.print(values[9], HEX);
    Serial.print(' ');
    Serial.print(values[10], HEX);
      }
}
void MODBUS2() 
{                                                                                         //modbus function to read the flowmeter value 
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

    buf3[0] = myHex1[7]; buf3[1] = myHex1[6]; buf3[2] = myHex1[5]; buf3[3] = myHex1[4];         //datas from flowmeter will be stored in buffer 
    memcpy((char *)&Flow1, buf3, 4);                                                         //in buf0 flow value will be stored
    value1 = (values1[7] << 8) | values1[6];
    value2 = (values1[5] << 8) | values1[4];
    value1_2 = (value1 << 16) | value2;
    Serial.print("value1_2******************************************");
    Serial.println(value1_2);
    Serial.print("BUFFER IS" );
    Serial.print(buf3[0] );
    buf4[0] = myHex1[11]; buf4[1] = myHex1[10]; buf4[2] = myHex1[9]; buf4[3] = myHex1[8];   
    memcpy((char *)&dataT3, buf4, 4);                                                       //in buf1 dataT1 will be stored
    Serial.println("data t3 is");
    Serial.println(dataT3);
    Serial.println("buf 4 data is");
    Serial.println(buf4);
    buf5[0] = myHex1[15]; buf5[1] = myHex1[14]; buf5[2] = myHex1[13]; buf5[3] = myHex1[12]; 
    memcpy((char *)&dataT4, buf5, 4);                                                       //in buf2 dataT2 will be stored
    
    Serial.print("dataT3 = ");
    Serial.println(dataT3);
    
    Serial.print("dataT4 = " );
    Serial.println(dataT4);
     Tot1 = ((65535 * dataT4) + dataT3);                                                   //totalizer formula given in datasheet
     Serial.println(Tot1);
    Serial.println("Flow Rate = "+String(Flow1));
    Serial.println("TOTALAIZER = "+String(Tot1));

    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("FLOW = " + String(Flow1) + " Ltr");
    lcd.setCursor(0, 1);
    lcd.print("TOT  = " + String(Tot1) + " Ltr");
    
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
