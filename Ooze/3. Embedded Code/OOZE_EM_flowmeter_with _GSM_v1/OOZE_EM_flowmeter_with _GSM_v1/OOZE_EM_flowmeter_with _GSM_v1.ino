#include "Dflux_FM.h"

void setup()
{

    mod.begin(9600);
    lcd_init();
    ModBus_Init();    
    GSM_Init();
    //WiFi_Init();
}

void loop()
{ 
    usbSerial.println(PSTR("No WIFI, Sending Data through GSM"));
    Send_Data_GSM();
}
