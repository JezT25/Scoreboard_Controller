/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#ifndef HARDWARE_h
#define HARDWARE_h

#include "../setup.hpp"

#define LED_FREQ			0.005
#define FETCH_INTERVAL 		25

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

#define TWO_DIGIT_DASH      110

class HARDWARE_class {
    private:
        volatile int CurrentSegment  = TENS_SEGMENT;
        Ticker timer;
        void IRAM_ATTR DisplayLED();

    protected:
        static volatile int Segment_1;
        static volatile int Segment_2;
        static volatile int Segment_3;
        static volatile int Colon_Flag;
        static volatile int Timeout_Flag;

    public:
        void Initialize();
};

#endif
