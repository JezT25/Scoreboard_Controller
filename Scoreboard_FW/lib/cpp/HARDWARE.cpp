/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

volatile int HARDWARE_class::Period         = FIRST_PERIOD;
volatile int HARDWARE_class::Posession      = 0;
volatile int HARDWARE_class::Time_Minute    = 0;
volatile int HARDWARE_class::Time_Seconds   = 0;
volatile int HARDWARE_class::Home_Score     = 0;
volatile int HARDWARE_class::Away_Score     = 0;
volatile int HARDWARE_class::Home_Fouls     = 0;
volatile int HARDWARE_class::Away_Fouls     = 0;
volatile int HARDWARE_class::Home_RTO       = 0;
volatile int HARDWARE_class::Away_RTO       = 0;
volatile int HARDWARE_class::pPeriod        = 10;
volatile int HARDWARE_class::pPosession     = 10;
volatile int HARDWARE_class::pHome_Score    = 200;
volatile int HARDWARE_class::pAway_Score    = 200;
volatile int HARDWARE_class::pHome_Fouls    = 20;
volatile int HARDWARE_class::pAway_Fouls    = 20;
volatile int HARDWARE_class::pHome_RTO      = 10;
volatile int HARDWARE_class::pAway_RTO      = 10;
volatile int HARDWARE_class::Colon_Flag     = GAME_MINUTE;
volatile bool HARDWARE_class::Power_Flag    = POWER_ON;
volatile bool HARDWARE_class::Clock_Flag    = LOW;

void IRAM_ATTR HARDWARE_class::DisplayLED() {
    if(Power_Flag == POWER_OFF)
    {
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
        digitalWrite(D4, LOW);
        digitalWrite(D5, LOW);
        digitalWrite(D6, LOW);
        digitalWrite(D7, LOW);
        digitalWrite(D8, LOW);
        digitalWrite(D9, LOW);
        digitalWrite(D10, LOW);
    }
    else
    {
        #if BOARD == 1
            // Clear
            digitalWrite(D0, HIGH);
            digitalWrite(D1, HIGH);
            digitalWrite(D2, HIGH);
            digitalWrite(D3, HIGH);
            digitalWrite(D4, HIGH);
            digitalWrite(D5, HIGH);
            digitalWrite(D6, HIGH);
            digitalWrite(D7, HIGH);
            digitalWrite(D8, LOW);
            digitalWrite(D9, LOW);
            digitalWrite(D10, LOW);

            // Set Digits
            digitalWrite(D8, (CurrentSegment & 1));
            digitalWrite(D9, (CurrentSegment & 2) >> 1);
            digitalWrite(D10, (CurrentSegment & 4) >> 2);

            if(CurrentSegment >= TENS_SEGMENT && CurrentSegment <= A_FOULRTO_SEGMENT)
            {
                int TENS, ONES;
                switch (CurrentSegment)
                {
                    case TENS_SEGMENT:
                        TENS = (Time_Minute / 10) ?: DISABLE_DIGIT;
                        ONES = Time_Minute % 10;
                        break;
                    case ONES_SEGMENT:
                        TENS = Time_Seconds / 10;
                        ONES = Colon_Flag == GAME_SECONDS ? DISABLE_DIGIT : Time_Seconds % 10;
                        break;
                    case H_SCORE_SEGMENT:
                        TENS = (Home_Score % 100) / 10;
                        ONES = Home_Score % 10;
                        if(Clock_Flag == HIGH && pHome_Score == Home_Score) TENS = ONES = DISABLE_DIGIT;
                        else if(Clock_Flag == HIGH && pHome_Score != Home_Score) pHome_Score = 200;
                        break;
                    case A_SCORE_SEGMENT:
                        TENS = (Away_Score % 100) / 10;
                        ONES = Away_Score % 10;
                        if(Clock_Flag == HIGH && pAway_Score == Away_Score) TENS = ONES = DISABLE_DIGIT;
                        else if(Clock_Flag == HIGH && pAway_Score != Away_Score) pAway_Score = 200;
                        break;
                    case H_FOULRTO_SEGMENT:
                        TENS = Home_Fouls;
                        ONES = Home_RTO ?: DISABLE_DIGIT;
                        if(Clock_Flag == HIGH && pHome_Fouls == Home_Fouls) TENS = DISABLE_DIGIT;
                        else if(Clock_Flag == HIGH && pHome_Fouls != Home_Fouls) pHome_Fouls = 20;
                        if(Clock_Flag == HIGH && pHome_RTO == Home_RTO) ONES = DISABLE_DIGIT;
                        else if(Clock_Flag == HIGH && pHome_RTO != Home_RTO) pHome_RTO = 10;
                        break;
                    case A_FOULRTO_SEGMENT:
                        TENS = Away_Fouls;
                        ONES = Away_RTO ?: DISABLE_DIGIT;
                        if(Clock_Flag == HIGH && pAway_Fouls == Away_Fouls) TENS = DISABLE_DIGIT;
                        else if(Clock_Flag == HIGH && pAway_Fouls != Away_Fouls) pAway_Fouls = 20;
                        if(Clock_Flag == HIGH && pAway_RTO == Away_RTO) ONES = DISABLE_DIGIT;
                        else if(Clock_Flag == HIGH && pAway_RTO != Away_RTO) pAway_RTO = 10;
                        break;
                }

                digitalWrite(D0, (TENS & 1));
                digitalWrite(D1, (TENS & 2) >> 1);
                digitalWrite(D2, (TENS & 4) >> 2);
                digitalWrite(D3, (TENS & 8) >> 3);
                digitalWrite(D4, (ONES & 1));
                digitalWrite(D5, (ONES & 2) >> 1);
                digitalWrite(D6, (ONES & 4) >> 2);
                digitalWrite(D7, (ONES & 8) >> 3);
            }
            else if(CurrentSegment == PERIOD_SEGMENT)
            {
                if(Clock_Flag == HIGH && pPeriod == Period) Period = NO_PERIOD;
                else if(Clock_Flag == HIGH && pPeriod != Period) pPeriod = 10;

                int p = (Period == NO_PERIOD)   ? 15 :
                (Period == FIRST_PERIOD)        ? 1  :
                (Period == SECOND_PERIOD)       ? 2  :
                (Period == THIRD_PERIOD)        ? 3  :
                (Period == FOURTH_PERIOD)       ? 4  :
                /* FIFTH_PERIOD */                1;

                digitalWrite(D0, (p & 1));
                digitalWrite(D1, (p & 2) >> 1);
                digitalWrite(D2, (p & 4) >> 2);
                digitalWrite(D3, (p & 8) >> 3);
                digitalWrite(D4, LOW);
                digitalWrite(D5, LOW);
                digitalWrite(D6, LOW);
                digitalWrite(D7, Period == FIFTH_PERIOD);
            }

            CurrentSegment = CurrentSegment == PERIOD_SEGMENT ? TENS_SEGMENT : ++CurrentSegment;
        #elif BOARD == 2
            if(Clock_Flag == HIGH)
            {
                if(pPosession == Posession) Posession = NO_POSESSION;
                else if(pPosession != Posession) pPosession = 10;
                if(pHome_Score != Home_Score) pHome_Score = 200;
                if(pAway_Score != Away_Score) pAway_Score = 200;
                if(pHome_Fouls != Home_Fouls) pHome_Fouls = 20;
                if(pAway_Fouls != Away_Fouls) pAway_Fouls = 20;
            }

            digitalWrite(D0, Colon_Flag == GAME_MINUTE);
            digitalWrite(D1, Colon_Flag != GAME_HIDE);
            digitalWrite(D2, Posession == HOME_POSESSION);
            digitalWrite(D3, Posession == AWAY_POSESSION);
            digitalWrite(D4, (Clock_Flag == HIGH && pHome_Score == Home_Score) ? LOW : Home_Score > 99);
            digitalWrite(D5, (Clock_Flag == HIGH && pAway_Score == Away_Score) ? LOW : Away_Score > 99);
            digitalWrite(D6, (Clock_Flag == HIGH && pHome_Fouls == Home_Fouls) ? LOW : Home_Fouls == DIGIT_P);
            digitalWrite(D7, (Clock_Flag == HIGH && pAway_Fouls == Away_Fouls) ? LOW : Away_Fouls == DIGIT_P);
            digitalWrite(D8, LOW);
            digitalWrite(D9, LOW);
            digitalWrite(D10, LOW);
        #endif
    }
}

void HARDWARE_class::Initialize() {
    noInterrupts();
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
    pinMode(D9, OUTPUT);
    pinMode(D10, OUTPUT);
    timer.attach(LED_FREQ, [this]() { this->DisplayLED(); });
    interrupts();
}
