/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#include "lib/setup.hpp"

void setup()
{
    HARDWARE.Initialize();
    WIFI.Initialize();
}

void loop()
{
	yield();
}
