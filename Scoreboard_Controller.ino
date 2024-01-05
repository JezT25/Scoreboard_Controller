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
	TIME.Function();
}

ISR(TIMER5_COMPA_vect)
{
	LED.DisplayDigits();
}

void loop()
{
	HARDWARE.Listener();
	LED.RefreshBuffer();
	BUTTON.Function();
}
