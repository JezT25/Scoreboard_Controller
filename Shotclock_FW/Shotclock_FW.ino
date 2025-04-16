/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#define WIFI_SSID1      "MyNodeMCU_AP"
#define WIFI_SSID2      "MyNodeMCU_AP1"
#define WIFI_PASSWORD1  "password123"
#define WIFI_PASSWORD2  "password123"

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
