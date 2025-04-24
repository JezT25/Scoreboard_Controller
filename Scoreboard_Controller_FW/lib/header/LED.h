/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef LED_h
#define LED_h

#include "../setup.hpp"

#define BLINK_INTERVAL 300
#define BLINK_HALFSEC  500

class LED_class : private HARDWARE_class {
    private:
        bool coldBoot = true;
        volatile unsigned int pTimeClock;
        volatile unsigned int currentSegment;
        volatile unsigned int pScore_Home;
        volatile unsigned int pFoul_Home;
        volatile unsigned int pTimeout_Home;
        volatile unsigned int pScore_Away;
        volatile unsigned int pFoul_Away;
        volatile unsigned int pTimeout_Away;
        volatile unsigned int pShotclock;
        volatile unsigned int pTime_Minute;
        volatile unsigned int pTime_Second;
        volatile unsigned int pClock_Hour;
        volatile unsigned int pClock_Minute;
        volatile unsigned int pPeriod;
        volatile unsigned int pPosession;
        volatile unsigned int pDots;
        volatile unsigned long lastBlinkTime;
        volatile bool BlinkState;
        volatile int Segment_Buffer[4][4][7];
        volatile int Dots_Buffer[2][4];
        const int LED_digitValues[13] =
        {
            B0111111, // 0
            B0000110, // 1
            B1011011, // 2
            B1001111, // 3
            B1100110, // 4
            B1101101, // 5
            B1111101, // 6
            B0000111, // 7
            B1111111, // 8
            B1101111, // 9
            B0000000, // All Off
            B1000000, // -
            B1110011  // P
        };
        enum LED_Displays
        {
            FOUL_HOME,
            TIMEOUT_HOME,
            FOUL_AWAY,
            TIMEOUT_AWAY,
            SHOTCLOCK_TENS      = 10,
            SHOTCLOCK_ONES,
            SCORE_HOME_TENS     = 20,
            SCORE_HOME_ONES,
            SCORE_AWAY_TENS,
            SCORE_AWAY_ONES,
            TIME_MIN_TENS       = 30,
            TIME_MIN_ONES,
            TIME_TENS,
            TIME_ONES
        };

        void UpdateMiniBuffer();
        void UpdateBuffer(int number, int location);
        void ClearBuffer();

    public:
        void RefreshBuffer();
        void DisplayDigits();
};

#endif
