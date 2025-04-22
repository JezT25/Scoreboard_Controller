/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#ifndef setup_cpp_included
#define setup_cpp_included

/** LIBRARIES **/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>

#include "header/HARDWARE.h"
#include "cpp/HARDWARE.cpp"
HARDWARE_class HARDWARE;

#include "header/WIFI.h"
#include "cpp/WIFI.cpp"
WIFI_class WIFI;

#endif
