#include <stdio.h>        //input output library
#include <stdint.h>       //integer type library
#include <string.h>       //string library
#include <Wire.h>         //To communicate with i2c


#include <SoftwareSerial.h>  
 
const byte requestframe2[] = {0x0A, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC4, 0xB3};
SoftwareSerial mod(32,33); // rx,tx
char  buf1[4], buf2[4];
int buf0[4];
float Flow, dataT1, dataT2 ,Tot = 0.0;
char buf3[4], buf4[4], buf5[4];
float Flow1, dataT3, dataT4 ,Tot1 = 0.0;
int hydro_result = 0;
byte myHex1[18];
byte values1[18];

void setup()
{
    Serial.begin(115200);
    mod.begin(9600);
    //if (DEREPin > 0) pinMode(DEREPin, OUTPUT);   

  delay(1000);     
    //GSM_Init();
    //WiFi_Init();
}

void loop()
{ memset(buf0,0,sizeof(buf0));
  memset(buf1,0,sizeof(buf1));
  memset(buf2,0,sizeof(buf2));
  if (mod.write(requestframe2, sizeof(requestframe2)) == 8) 
  {
    for (byte i = 0; i <18; i++) 
    {
      values1[i] = mod.read();
      myHex1[i] = values1[i];
      delay(10);
    }

    uint32_t myLongNum = (myHex1[7] << 24) | (myHex1[6] << 16) | (myHex1[5] << 8) | (myHex1[4]);
    union{
      uint32_t intValue;
      float floatValue;
    }data;
    data.intValue = myLongNum;
    Flow1 = data.floatValue;    
    Serial.println("Flow Rate = "+String(Flow1));
    Serial.print("myLongNum...");
    Serial.println(myLongNum);
    Serial.print("data.intValue...");
    Serial.println(data.intValue);
    buf4[0] = myHex1[11]; buf4[1] = myHex1[10]; buf4[2] = myHex1[9]; buf4[3] = myHex1[8];   
    memcpy((char *)&dataT3, buf4, 4);                                                       //in buf1 dataT1 will be stored
    
    buf5[0] = myHex1[15]; buf5[1] = myHex1[14]; buf5[2] = myHex1[13]; buf5[3] = myHex1[12]; 
    memcpy((char *)&dataT4, buf5, 4);                                                       //in buf2 dataT2 will be stored
    
    /*Serial.print("dataT3 = ");
    Serial.println(dataT3);
    Serial.println(" ");
    Serial.print("buf 4 data is..........");
    Serial.println(buf4);
    Serial.println(" ");
    Serial.print("dataT4 = " );
    Serial.println(dataT4);
    Serial.println(" ");
    Serial.print("buf 5 data is..........");
    Serial.println(buf5);
    Serial.println(" ");
    Tot1 = ((65535 * dataT4) + dataT3);  */                                                 //totalizer formula given in datasheet
    //Serial.println(Tot1);
    //Serial.println("Flow Rate = "+String(Flow1));
    //Serial.println("TOTALAIZER = "+String(Tot1));
    
    Serial.print(values1[0], HEX);                                                         
    Serial.print(' ');
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
