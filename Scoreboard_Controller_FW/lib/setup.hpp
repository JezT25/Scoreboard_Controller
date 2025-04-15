/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#ifndef setup_cpp_included
#define setup_cpp_included

/** LIBRARIES **/
#include <Arduino.h>
#include "LowPower.h"
#include <DS3231RinkyDink.h>
DS3231RinkyDink rtc(SDA, SCL);

#include "IDEVICE.h"
IDATA IData;
ISYSTEM ISystem;
IHARDWARE IHardware;

#include "header/HARDWARE.h"
#include "cpp/HARDWARE.cpp"
HARDWARE_class HARDWARE;

#include "header/LED.h"
#include "cpp/LED.cpp"
LED_class LED;

#include "header/TIME.h"
#include "cpp/TIME.cpp"
TIME_class TIME;

#include "header/BUTTON.h"
#include "cpp/BUTTON.cpp"
BUTTON_class BUTTON;

#include "header/WIFI.h"
#include "cpp/WIFI.cpp"
WIFI_class WIFI;

#endif
