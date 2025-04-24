/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

/* 
 * Choose the board you are using by setting the following:
 *   - If you are using Board 1, set:  #define BOARD 1
 *   - If you are using Board 2, set:  #define BOARD 2
 * 
 * Only **one** of the options should be set at a time. 
 * Change the correct line based on your setup.
 */

#define BOARD 1

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
