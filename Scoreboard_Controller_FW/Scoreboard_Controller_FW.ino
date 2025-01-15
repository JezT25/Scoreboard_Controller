/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#include "lib/setup.hpp"

void setup()
{
	HARDWARE.Initialize();
}

ISR(TIMER4_COMPA_vect)
{
	TIME.TimeUpdate();
}

ISR(TIMER5_COMPA_vect)
{
	LED.DisplayDigits();
}

void loop()
{
	HARDWARE.Listener();
	TIME.Function();
	LED.RefreshBuffer();
	BUTTON.Function();
	WIFI.SendUpdate();
}
