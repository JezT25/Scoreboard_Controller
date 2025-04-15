/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#ifndef IDEVICE_h
#define IDEVICE_h

#include "setup.hpp"

#define NO_PERIOD           16
#define FIRST_PERIOD        8
#define SECOND_PERIOD       4
#define THIRD_PERIOD        2
#define FOURTH_PERIOD       1

#define HOME_POSESSION      8
#define AWAY_POSESSION      4
#define NO_POSESSION        0

#define GAME_MINUTE         3
#define GAME_SECONDS        1
#define GAME_HIDE           0

#define DIGIT_OFF           10
#define DIGIT_DASH          11
#define TWO_DIGIT_OFF       100
#define TWO_DIGIT_DASH      110

#define TIME_PAUSE          1
#define TIME_RUNNING        2
#define TIME_ADJUST         3
#define TIME_RESET          4
#define TIME_CLOCK          5
#define TIME_CLOCKADJUST    6

#define POWER_OFF           0
#define POWER_ON            1

class IDATA {
    public:
        volatile unsigned int SCORE_HOME        =    0;

        volatile unsigned int FOUL_HOME         =    0;

        volatile unsigned int TIMEOUT_HOME      =    4;

        volatile unsigned int SCORE_AWAY        =    0;

        volatile unsigned int FOUL_AWAY         =    0;

        volatile unsigned int TIMEOUT_AWAY      =    4;

        volatile unsigned int SHOTCLOCK         =    24;

        volatile unsigned int TIME_MINUTE       =    10;

        volatile unsigned int TIME_SECOND       =    0;

        volatile unsigned int TIME_MS           =    0;

        volatile unsigned int TIME_SC_MS        =    0;
        
        volatile unsigned int CLOCK_HOUR        =    0;
        
        volatile unsigned int CLOCK_MINUTE      =    0;

        volatile unsigned int GAME_PERIOD       =    FIRST_PERIOD;

        volatile unsigned int GAME_POSESSION    =    NO_POSESSION;

        volatile unsigned int GAME_DOTS         =    GAME_MINUTE;

        volatile bool TIMEOUT_FLAG              =    LOW;

        String toJSON();
};

class ISYSTEM {
    public:
        volatile unsigned int TIME_MODE         =    TIME_PAUSE;

        volatile unsigned int SC_TIME_MODE      =    TIME_RESET;

        volatile unsigned int POWER_STATE       =    POWER_ON;
};

class IHARDWARE {
    public:
        volatile unsigned int beepTime;

        volatile unsigned int honkTime;

        volatile unsigned long lastBeepTime;

        volatile unsigned long lastHonkTime;
};

#endif
