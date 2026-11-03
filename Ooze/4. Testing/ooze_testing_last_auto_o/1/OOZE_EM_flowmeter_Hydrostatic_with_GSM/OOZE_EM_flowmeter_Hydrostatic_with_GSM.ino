#include "Dflux_FM.h"

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

}
