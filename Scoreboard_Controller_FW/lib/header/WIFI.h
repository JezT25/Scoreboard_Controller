/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef WIFI_h
#define WIFI_h

#include "../setup.hpp"

#define WIFI_INTERVAL 15

class WIFI_class : private HARDWARE_class {
    private:
        unsigned long lastWiFiUpdate = 0;

    public:
        void SendUpdate();
};

#endif
