#include "Dflux_FM.h"
unsigned long restartInterval = 0;

void setup()
{
    mod.begin(9600);
    lcd_init();
    ModBus_Init();    
    GSM_Init();
}

void loop()
{ 
  
    Send_Data_GSM();
    restartInterval++;
    if(restartInterval == 3600000){
    Serial.println("Restarting ESP32...");
    ESP.restart();  // Restart ESP32
    }
}
