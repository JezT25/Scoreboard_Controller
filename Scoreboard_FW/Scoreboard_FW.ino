/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#define WIFI_SSID 		"MyNodeMCU_AP"
#define WIFI_PASSWORD 	"password123"

#include "lib/setup.hpp"

void setup()
{
    HARDWARE.Initialize();
    WIFI.Initialize();
}

void loop()
{   
    WIFI.GetUpdate();
}
