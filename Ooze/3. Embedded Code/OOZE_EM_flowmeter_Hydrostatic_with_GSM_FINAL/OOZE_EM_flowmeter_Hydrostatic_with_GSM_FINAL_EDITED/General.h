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
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int counter = 0;
const byte requestframe[] = { 0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC5, 0xCB };  //assigning request frame values
const byte requestframe2[] = { 0x05, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC4, 0x4C };
//{0x0B, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0x62};

SoftwareSerial mod(32, 33);  // rx,tx
char buf0[4], buf1[4], buf2[4];
float Flow, dataT1, dataT2, Tot = 0.0;
char buf3[4], buf4[4], buf5[4];
float Flow1, dataT3, dataT4, Tot1 = 0.0;
int hydro_result = 0;
byte myHex1[18];
byte values1[18];
byte myHex[10];
byte values[10];
int Raw, Reset, count = 0;
String sensorvalue = "";  //hydrostatic sensor
                          //int hydro_result;

void lcd_init() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("GOWATR  SWMS");
}

void lcd_data_sent() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("DATA  SENT");
}
void lcd_data_fail() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("DATA  FAIL");
}


void MODBUS() {  //modbus function to read the flowmeter value
  Serial.println(" ");
  Serial.println("inside modbus........................1");
  memset(values, 0, sizeof(values));
  memset(buf3, 0, sizeof(buf3));  //clearing the buffers
  memset(buf4, 0, sizeof(buf4));
  memset(buf5, 0, sizeof(buf5));
  memset(values1, 0, sizeof(values1));
  memset(myHex1, 0, sizeof(myHex1));

  if (mod.write(requestframe, sizeof(requestframe)) == 8)  //first request frame will be given as input to flowmeter
  {
    delay(1000);
    for (byte i = 1; i <= 10; i++) {  //response from flowmeter will be read here
      values[i] = mod.read();
      //myHex[i] = values[i];
      delay(10);
    }
      if (values[1] == 1 && values[2] == 3) {
      Serial.print("the value of 4 is ");
      Serial.println(values[4], HEX);
      Serial.print("the value of 5 is ");
      Serial.println(values[5], HEX);
      hydro_result = (values[4] << 8) | values[5];  //This line has to be implemented for concating. (Bitwise operation)
      Serial.print("The SOLVED RESULT is ");
      Serial.println(hydro_result);
    }
    else if (values[2] == 1 && values[3] == 3) {
      Serial.print("the value of 5 is ");
      Serial.println(values[5], HEX);
      Serial.print("the value of 6 is ");
      Serial.println(values[6], HEX);
      hydro_result = (values[5] << 8) | values[6];  //This line has to be implemented for concating. (Bitwise operation)
      Serial.print("The SOLVED RESULT is ");
      Serial.println(hydro_result);
    } else if (values[3] == 1 && values[4] == 3) {
      Serial.print("the value of 5 is ");
      Serial.println(values[6], HEX);
      Serial.print("the value of 6 is ");
      Serial.println(values[7], HEX);
      hydro_result = (values[6] << 8) | values[7];  //This line has to be implemented for concating. (Bitwise operation)
      Serial.print("The SOLVED RESULT is ");
      Serial.println(hydro_result);
    } else if (values[4] == 1 && values[5] == 3) {
      Serial.print("the value of 5 is ");
      Serial.println(values[7], HEX);
      Serial.print("the value of 6 is ");
      Serial.println(values[8], HEX);
      hydro_result = (values[7] << 8) | values[8];  //This line has to be implemented for concating. (Bitwise operation)
      Serial.print("The SOLVED RESULT is ");
      Serial.println(hydro_result);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" HS Sensor :");
    lcd.setCursor(12, 0);
    lcd.print(hydro_result);
    lcd.setCursor(0, 1);
    lcd.print("Water Level:");
    delay(2000);

    Serial.print("Received values: ");
    for (byte i = 1; i <= 10; i++) {
      Serial.print(values[i], HEX);
      Serial.print(' ');
    }
    Serial.println(" ");
  }
}
void MODBUS2() {  //modbus function to read the flowmeter value
  Serial.println(" ");
  Serial.println("inside modbus::::::::::::::::::2");

  if (mod.write(requestframe2, sizeof(requestframe2)) == 8)  //first request frame will be given as input to flowmeter
  {
    delay(1000);
    for (byte i = 0; i < 18; i++) {  //response from flowmeter will be read here
      values1[i] = mod.read();
      myHex1[i] = values1[i];
      delay(10);
    }
    // delay(30);
    // mod.flush();
    // delay(30);
    if (values1[0] == 5 && values1[1] == 3) {
      buf3[0] = myHex1[6];
      buf3[1] = myHex1[5];
      buf3[2] = myHex1[4];
      buf3[3] = myHex1[3];              //datas from flowmeter will be stored in buffer
      memcpy((char *)&Flow1, buf3, 4);  //in buf0 flow value will be stored

      buf4[0] = myHex1[10];
      buf4[1] = myHex1[9];
      buf4[2] = myHex1[8];
      buf4[3] = myHex1[7];
      memcpy((char *)&dataT3, buf4, 4);  //in buf1 dataT1 will be stored

      buf5[0] = myHex1[14];
      buf5[1] = myHex1[13];
      buf5[2] = myHex1[12];
      buf5[3] = myHex1[11];
      memcpy((char *)&dataT4, buf5, 4);  //in buf2 dataT2 will be stored

      Tot1 = ((65535 * dataT4) + dataT3);  //totalizer formula given in datasheet
      Serial.println("Flow Rate = " + String(Flow1));
      Serial.println("TOTALAIZER = " + String(Tot1));
      counter = 0;
    } else if (values1[1] == 5 && values1[2] == 3) {
      buf3[0] = myHex1[7];
      buf3[1] = myHex1[6];
      buf3[2] = myHex1[5];
      buf3[3] = myHex1[4];              //datas from flowmeter will be stored in buffer
      memcpy((char *)&Flow1, buf3, 4);  //in buf0 flow value will be stored

      buf4[0] = myHex1[11];
      buf4[1] = myHex1[10];
      buf4[2] = myHex1[9];
      buf4[3] = myHex1[8];
      memcpy((char *)&dataT3, buf4, 4);  //in buf1 dataT1 will be stored

      buf5[0] = myHex1[15];
      buf5[1] = myHex1[14];
      buf5[2] = myHex1[13];
      buf5[3] = myHex1[12];
      memcpy((char *)&dataT4, buf5, 4);  //in buf2 dataT2 will be stored

      Tot1 = ((65535 * dataT4) + dataT3);  //totalizer formula given in datasheet
      Serial.println("Flow Rate = " + String(Flow1));
      Serial.println("TOTALAIZER = " + String(Tot1));
      counter = 0;
    } else if (values1[2] == 5 && values1[3] == 3) {
      buf3[0] = myHex1[8];
      buf3[1] = myHex1[7];
      buf3[2] = myHex1[6];
      buf3[3] = myHex1[5];              //datas from flowmeter will be stored in buffer
      memcpy((char *)&Flow1, buf3, 4);  //in buf0 flow value will be stored

      buf4[0] = myHex1[12];
      buf4[1] = myHex1[11];
      buf4[2] = myHex1[10];
      buf4[3] = myHex1[9];
      memcpy((char *)&dataT3, buf4, 4);  //in buf1 dataT1 will be stored

      buf5[0] = myHex1[16];
      buf5[1] = myHex1[15];
      buf5[2] = myHex1[14];
      buf5[3] = myHex1[13];
      memcpy((char *)&dataT4, buf5, 4);  //in buf2 dataT2 will be stored

      Tot1 = ((65535 * dataT4) + dataT3);  //totalizer formula given in datasheet
      Serial.println("Flow Rate = " + String(Flow1));
      Serial.println("TOTALAIZER = " + String(Tot1));
      counter = 0;
    } else if (values1[3] == 5 && values1[4] == 3) {
      buf3[0] = myHex1[9];
      buf3[1] = myHex1[8];
      buf3[2] = myHex1[7];
      buf3[3] = myHex1[6];              //datas from flowmeter will be stored in buffer
      memcpy((char *)&Flow1, buf3, 4);  //in buf0 flow value will be stored

      buf4[0] = myHex1[13];
      buf4[1] = myHex1[12];
      buf4[2] = myHex1[11];
      buf4[3] = myHex1[10];
      memcpy((char *)&dataT3, buf4, 4);  //in buf1 dataT1 will be stored

      buf5[0] = myHex1[17];
      buf5[1] = myHex1[16];
      buf5[2] = myHex1[15];
      buf5[3] = myHex1[14];
      memcpy((char *)&dataT4, buf5, 4);  //in buf2 dataT2 will be stored

      Tot1 = ((65535 * dataT4) + dataT3);  //totalizer formula given in datasheet
      Serial.println("Flow Rate = " + String(Flow1));
      Serial.println("TOTALAIZER = " + String(Tot1));
      counter = 0;
    } else {
      counter++;
      if (counter >= 20) {
        counter = 0;
        Tot1 = -1;
        Flow1 = -1;
      }
    }
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("FLOW = " + String(Flow1) + " Ltr");
    lcd.setCursor(0, 1);
    lcd.print("TOT  = " + String(Tot1) + " Ltr");

    Serial.print("Received values1: ");
    for (byte i = 0; i < 18; i++) {
      Serial.print(values1[i], HEX);
      Serial.print(' ');
    }
    Serial.println();
  }
}
