/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef WIFI_h
#define WIFI_h

#include "../setup.hpp"
#include "../../PROTOCOL.h"

#define WIFI_INTERVAL 20

class WIFI_class : private HARDWARE_class {
    private:
        unsigned long lastWiFiUpdate = 0;

        void BuildPacket(ScoreboardPacket &packet);

    public:
        void StartPairing();
        void ReadPairingStatus();
        void SendUpdate();
};

#endif
