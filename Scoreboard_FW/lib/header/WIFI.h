/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef WIFI_h
#define WIFI_h

#include "../setup.hpp"

class WIFI_class : private HARDWARE_class {
	private:
		static bool nc;
		static bool coldboot;
		static void OnDataRecv(uint8_t *mac, uint8_t *data, uint8_t len);

    public:
		void Initialize();
};

#endif
