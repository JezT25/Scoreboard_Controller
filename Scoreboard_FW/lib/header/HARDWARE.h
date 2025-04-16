/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef HARDWARE_h
#define HARDWARE_h

#include "../setup.hpp"

#define LED_FREQ			0.0025
#define FETCH_INTERVAL 		25

#define DISABLE_DIGIT		15

#define NO_PERIOD           16
#define FIRST_PERIOD        8
#define SECOND_PERIOD       4
#define THIRD_PERIOD        2
#define FOURTH_PERIOD       1

#define HOME_POSESSION      8
#define AWAY_POSESSION      4
#define NO_POSESSION        0

#define GAME_HIDE       	0
#define GAME_SECONDS    	1
#define GAME_MINUTE     	3

#define PERIOD_SEGMENT      0
#define TENS_SEGMENT		1
#define ONES_SEGMENT		2
#define H_SCORE_SEGMENT		3
#define A_SCORE_SEGMENT	    4
#define H_FOULRTO_SEGMENT   5
#define A_FOULRTO_SEGMENT	6
#define POS_SEGMENT		    7

#define POWER_OFF           0
#define POWER_ON            1

class HARDWARE_class {
    private:
        const int segmentPins[8]     = { D0, D1, D2, D3, D4, D5, D6, D7 };
        const byte digitPatterns[11] = {
            0b00111111, // 0
            0b00000110, // 1
            0b01011011, // 2
            0b01001111, // 3
            0b01100110, // 4
            0b01101101, // 5
            0b01111101, // 6
            0b00000111, // 7
            0b01111111, // 8
            0b01101111, // 9
            0b00000000  // Nothing
        };
        volatile int CurrentSegment  = TENS_SEGMENT;
        Ticker timer;
        void IRAM_ATTR DisplayLED();

    protected:
        static volatile int pPeriod;
        static volatile int pPosession;
        static volatile int pHome_Score;
        static volatile int pAway_Score;
        static volatile int pHome_Fouls;
        static volatile int pAway_Fouls;
        static volatile int pHome_RTO;
        static volatile int pAway_RTO;
        static volatile int Period;
        static volatile int Posession;
        static volatile int Time_Minute;
        static volatile int Time_Seconds;
        static volatile int Home_Score;
        static volatile int Away_Score;
        static volatile int Home_Fouls;
        static volatile int Away_Fouls;
        static volatile int Home_RTO;
        static volatile int Away_RTO;
        static volatile int Colon_Flag;
        static volatile bool Power_Flag;
        static volatile bool Clock_Flag;

    public:
        void Initialize();
};

#endif
