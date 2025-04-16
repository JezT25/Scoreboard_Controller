/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef WIFI_h
#define WIFI_h

#include "../setup.hpp"

#define SERVER		        "http://192.168.4.1:80/"
#define CONNECTING_INTERVAL 1000

class WIFI_class : private HARDWARE_class {
	private:
		bool coldboot = true;
		HTTPClient	http;
		WiFiClient	client;

    public:
		void Initialize();
        void GetUpdate();
};

#endif
