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
volatile int HARDWARE_class::Colon_Flag     = GAME_MINUTE;

void IRAM_ATTR HARDWARE_class::DisplayLED() {
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

    int TENS, ONES;

    switch (CurrentSegment)
    {
        case TENS_SEGMENT:
            TENS = Time_Minute / 10;
            ONES = Time_Minute % 10;
            break;
        case ONES_SEGMENT:
            TENS = Time_Seconds / 10;
            ONES = Colon_Flag == GAME_MINUTE ? Time_Seconds % 10 : DISABLE_DIGIT;
            break;
        case H_SCORE_SEGMENT:
            TENS = (Home_Score % 100) / 10;
            ONES = Home_Score % 10;
            break;
        case A_SCORE_SEGMENT:
            TENS = (Away_Score % 100) / 10;
            ONES = Away_Score % 10;
            break;
        case H_FOULRTO_SEGMENT:
            TENS = Home_Fouls;
            ONES = Home_RTO;
            break;
        case A_FOULRTO_SEGMENT:
            TENS = Away_Fouls;
            ONES = Away_RTO;
            break;
    }

    // Set Digits
    digitalWrite(D8, (CurrentSegment & 1));
    digitalWrite(D9, (CurrentSegment & 2) >> 1);
    digitalWrite(D10, (CurrentSegment & 4) >> 2);

    if(CurrentSegment > PERIOD_SEGMENT && CurrentSegment < POS_SEGMENT)
    {
        if(Colon_Flag == GAME_HIDE) TENS = ONES = DISABLE_DIGIT;        

        digitalWrite(D0, (TENS & 1));
        digitalWrite(D1, (TENS & 2) >> 1);
        digitalWrite(D2, (TENS & 4) >> 2);
        digitalWrite(D3, (TENS & 8) >> 3);
        digitalWrite(D4, (ONES & 1));
        digitalWrite(D5, (ONES & 2) >> 1);
        digitalWrite(D6, (ONES & 4) >> 2);
        digitalWrite(D7, (ONES & 8) >> 3);
    }
    else if(Colon_Flag == GAME_HIDE)
    {
        digitalWrite(D0, 0);
        digitalWrite(D1, 0);
        digitalWrite(D2, 0);
        digitalWrite(D3, 0);
        digitalWrite(D4, 0);
        digitalWrite(D5, 0);
        digitalWrite(D6, 0);
        digitalWrite(D7, 0);
    }
    else if(CurrentSegment == PERIOD_SEGMENT)
    {
        byte pattern = digitPatterns[(Period == NO_PERIOD) ? 10 : (Period == FIRST_PERIOD) ? 1 : (Period == SECOND_PERIOD) ? 2 : (Period == THIRD_PERIOD) ? 3 : (Period == FOURTH_PERIOD) ? 4 : 1];
        for (int i = 0; i < 7; i++) {
            digitalWrite(segmentPins[i], pattern & (1 << i) ? HIGH : LOW);
        }
    }
    else if(CurrentSegment == POS_SEGMENT)
    {
        digitalWrite(D0, Colon_Flag == GAME_MINUTE);
        digitalWrite(D1, Colon_Flag != GAME_HIDE);
        digitalWrite(D2, Posession == HOME_POSESSION);
        digitalWrite(D3, Posession == AWAY_POSESSION);
        digitalWrite(D4, Home_Score > 99);
        digitalWrite(D5, 0);
        digitalWrite(D6, Away_Score > 99);
        digitalWrite(D7, 0);
    }

    CurrentSegment = CurrentSegment == POS_SEGMENT ? PERIOD_SEGMENT : ++CurrentSegment;
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
