/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef HARDWARE_h
#define HARDWARE_h

#include "../setup.hpp"

#define BUZZER          2
#define HORN            39

#define FT_A            53
#define FT_B            52
#define FT_C            50
#define FT_D            48
#define FT_E            46
#define FT_F            44
#define FT_G            42

#define SC_A            32
#define SC_B            30
#define SC_C            28
#define SC_D            26
#define SC_E            24
#define SC_F            22
#define SC_G            23

#define S_A             A0
#define S_B             A1
#define S_C             A2
#define S_D             A3
#define S_E             A4
#define S_F             A5
#define S_G             A6

#define T_A             A8
#define T_B             A9
#define T_C             A10
#define T_D             A11
#define T_E             A12
#define T_F             A13
#define T_G             A14

#define L_P             A7
#define L_C             A15

#define SECTION_1       34
#define SECTION_2       36
#define SECTION_3       38
#define SECTION_4       40

#define BUTTON_COUNT    14
#define HOME_UP         25
#define HOME_DOWN       27
#define HOME_FOUL       29
#define HOME_TIMEOUT    31
#define AWAY_UP         51
#define AWAY_DOWN       49
#define AWAY_FOUL       47
#define AWAY_TIMEOUT    45
#define TIME_MIN        33
#define TIME_SEC        43
#define TIME_BUTTON     35
#define SC_PRESET       37
#define SC_STARTSTOP    41
#define POWER_SW        13

#define BEEP_SHORT      50
#define BEEP_MED        100
#define BEEP_LONG       300
#define BEEP_EX_LONG    1000
#define BEEP_EXX_LONG   3000
#define TONE_LOW        800
#define TONE_HIGH       1000

#define SERIAL3_BAUD    115200

class HARDWARE_class {
    protected:
        const int LED_Section[4] = { SECTION_1, SECTION_2, SECTION_3, SECTION_4 };
        const int LED_Dots[2] = { L_P, L_C };
        const int Segment_7[4][7] =
        {
            { FT_A, FT_B, FT_C, FT_D, FT_E, FT_F, FT_G },       // Fouls & Timeouts - FT
            { SC_A, SC_B, SC_C, SC_D, SC_E, SC_F, SC_G },       // Shot Clock       - SC
            { S_A, S_B, S_C, S_D, S_E, S_F, S_G },              // Score            - S
            { T_A, T_B, T_C, T_D, T_E, T_F, T_G }               // Time             - T
        };
        const int buttonPins[BUTTON_COUNT] =
        {
            HOME_UP,
            HOME_DOWN,
            HOME_FOUL,
            HOME_TIMEOUT,
            AWAY_UP,
            AWAY_DOWN,
            AWAY_FOUL,
            AWAY_TIMEOUT,
            TIME_MIN,
            TIME_SEC,
            TIME_BUTTON,
            SC_PRESET,
            SC_STARTSTOP,
            POWER_SW
        };

        void Honk(int duration);
        void Beep(int duration, int frequency);
        void DisableNoise();

    private:
        void BUZZER_Initialize();
        void BUTTON_Initialize();
        void TIME_Initialize();
        void LED_Initialize();
        void WIFI_Initialize();
        void CheckForPower();

    public:
        void Initialize();
        void Listener();
};

#endif
