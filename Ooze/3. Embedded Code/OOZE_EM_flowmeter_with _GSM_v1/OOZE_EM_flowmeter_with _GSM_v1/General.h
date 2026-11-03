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
LiquidCrystal_I2C lcd(0x27,16,2); 
const byte requestframe[] = {0x0A, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC4, 0xB3};
//const byte requestframe[] = {0x0B, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0x62};
//{0x0A, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC4, 0xB3};
SoftwareSerial mod(32,33); // rx,tx
char buf0[4], buf1[4], buf2[4];
float Flow, dataT1, dataT2 ,Tot = 0.0;
byte myHex[18];
byte values[18];
int Raw, Reset, count = 0;

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
{
  memset(buf0,0,sizeof(buf0));
  memset(buf1,0,sizeof(buf1));
  memset(buf2,0,sizeof(buf2));
  if (mod.write(requestframe, sizeof(requestframe)) == 8) 
  {
    for (byte i = 0; i <18; i++) 
    {
      values[i] = mod.read();
      myHex[i] = values[i];
      delay(10);
    }

    buf0[0] = myHex[7]; buf0[1] = myHex[6]; buf0[2] = myHex[5]; buf0[3] = myHex[4]; 
    memcpy((char *)&Flow, buf0, 4); 
    Serial.print("BUFFER IS" );
    Serial.print(buf0[0] );
    buf1[0] = myHex[11]; buf1[1] = myHex[10]; buf1[2] = myHex[9]; buf1[3] = myHex[8]; 
    memcpy((char *)&dataT1, buf1, 4); 

    buf2[0] = myHex[15]; buf2[1] = myHex[14]; buf2[2] = myHex[13]; buf2[3] = myHex[12]; 
    memcpy((char *)&dataT2, buf2, 4); 
    
    Serial.print("dataT1 = ");
    Serial.println(dataT1);
    
    Serial.print("dataT2 = " );
    Serial.println(dataT2);
     Tot = ((65535 * dataT2) + dataT1);
   
    //ID1 = Flow;
    //ID2 = Tot;
    //sprintf((char*)ID1,"%.0f",Flow);
    //sprintf((char*)ID2,"%.0f",Tot);
    //Serial.println("Totalizer 1 = " + String(dataT1)); 
    //Serial.println("Totalizer 2 = " + String(dataT2));
    Serial.println("Flow Rate = " + String(Flow));
    //Serial.print("Flow rate = ");
    //Serial.println(ID1);
    Serial.println("TOTALAIZER = " + String(Tot));
    
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("FLOW = " + String(Flow) + " Ltr");
    lcd.setCursor(0, 1);
    lcd.print("TOT  = " + String(Tot) + " Ltr");
    
  
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
    Serial.print(' ');
    Serial.print(values[11], HEX);
    Serial.print(' ');
    Serial.print(values[12], HEX);
    Serial.print(' ');
    Serial.print(values[13], HEX);
    Serial.print(' ');
    Serial.print(values[14], HEX);
    Serial.print(' ');
    Serial.print(values[15], HEX);
    Serial.print(' ');
    Serial.print(values[16], HEX);
    Serial.print(' ');
    Serial.print(values[17], HEX);
    Serial.println(' ');
  }
}
