/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#ifndef HARDWARE_h
#define HARDWARE_h

#include "../setup.hpp"

#define LED_FREQ			0.0025
#define D3_LOW_HOLD_MS		1

#define DISABLE_DIGIT		15

#define TENS_SEGMENT		1
#define UPCOL_SEGMENT		2
#define ONES_SEGMENT		3
#define DWNCOL_SEGMENT		4
#define SC_SEGMENT			5
#define TOUT_SEGMENT		6

#define GAME_HIDE       	0
#define GAME_SECONDS    	1
#define GAME_MINUTE     	3

#define TWO_DIGIT_DASH      110

#define POWER_OFF           0
#define POWER_ON            1

class HARDWARE_class {
    private:
        volatile int CurrentSegment  = TENS_SEGMENT;
        Ticker timer;
        Ticker d3Timer;
        void IRAM_ATTR DisplayLED();
        // D3 doubles as the tens-digit bit-3 data line and the 4028 "D" address
        // input. It rests HIGH and is only pulsed LOW (briefly extended) when
        // the data actually requires it, instead of blanking the whole bus.
        void IRAM_ATTR SetTensBit3(bool high);

    protected:
        static volatile int Segment_1;
        static volatile int Segment_2;
        static volatile int Segment_3;
        static volatile int Colon_Flag;
        static volatile bool Timeout_Flag;
        static volatile bool Power_Flag;

    public:
        void Initialize();
};

#endif
