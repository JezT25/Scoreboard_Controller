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
#include <ESP8266HTTPClient.h>
#include <Ticker.h>

#define SERVER				"http://192.168.4.1:80/"
#define LED_FREQ			0.005
#define FETCH_INTERVAL 		100
#define CONNECTING_INTERVAL 1000

#define DISABLE_DIGIT		15

#define TENS_SEGMENT		1
#define UPCOL_SEGMENT		2
#define ONES_SEGMENT		3
#define DWNCOL_SEGMENT		4
#define SC_SEGMENT			5
#define TOUT_SEGMENT		6

#define TIMEOUT_OFF			0
#define TIMEOUT_ON			1

#define GAME_HIDE       	0
#define GAME_SECONDS    	1
#define GAME_MINUTE     	3

/** VARIABLES **/
volatile int Segment_1 		= 0;
volatile int Segment_2 		= 0;
volatile int Segment_3 		= 0;
volatile int CurrentSegment = TENS_SEGMENT;
volatile int Colon_Flag 	= GAME_MINUTE;
volatile int Timeout_Flag 	= TIMEOUT_OFF;

/** CLASS VARIABLES **/
Ticker 		timer;
HTTPClient 	http;
WiFiClient 	client;

/** LIBRARIES **/
#include "Shotclock_FW.cpp"

#endif
