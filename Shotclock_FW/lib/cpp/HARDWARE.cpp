/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

volatile int HARDWARE_class::Segment_1      = 0;
volatile int HARDWARE_class::Segment_2      = 0;
volatile int HARDWARE_class::Segment_3      = 0;
volatile int HARDWARE_class::Colon_Flag     = GAME_MINUTE;
volatile bool HARDWARE_class::Timeout_Flag  = LOW;
volatile bool HARDWARE_class::Power_Flag    = POWER_ON;

void IRAM_ATTR HARDWARE_class::GetDigits(int Segment, int &TENS, int &ONES) {
    if (Segment == SC_SEGMENT && Segment_3 == TWO_DIGIT_DASH)
    {
        TENS = DISABLE_DIGIT;
        ONES = DISABLE_DIGIT;
        return;
    }

    TENS = (Segment == TENS_SEGMENT ? Segment_1 : Segment == ONES_SEGMENT ? Segment_2 : Segment_3) / 10;
    ONES = (Segment == TENS_SEGMENT ? Segment_1 : Segment == ONES_SEGMENT ? Segment_2 : Segment_3) % 10;

    if(Segment == TENS_SEGMENT && TENS == 0) TENS = DISABLE_DIGIT;
    if(Segment == ONES_SEGMENT && Colon_Flag == GAME_SECONDS) ONES = DISABLE_DIGIT;
}

void IRAM_ATTR HARDWARE_class::DisplayLED() {
    if(Power_Flag != POWER_ON)
    {
        GPOC = (1 << D8) | (1 << D9) | (1 << D10);
        Phase = 0;
        return;
    }

    if (CurrentSegment == TENS_SEGMENT || CurrentSegment == ONES_SEGMENT || CurrentSegment == SC_SEGMENT) {
        if (Phase == 0)
        {
            int TENS, ONES;
            GetDigits(CurrentSegment, TENS, ONES);

            // Close the colon select, then change the bus while every LD is low
            GPOC = (1 << D8) | (1 << D9) | (1 << D10);
            digitalWrite(D0, (TENS & 1));
            digitalWrite(D1, (TENS & 2) >> 1);
            digitalWrite(D2, (TENS & 4) >> 2);
            digitalWrite(D3, (TENS & 8) >> 3);
            digitalWrite(D4, (ONES & 1));
            digitalWrite(D5, (ONES & 2) >> 1);
            digitalWrite(D6, (ONES & 4) >> 2);
            digitalWrite(D7, (ONES & 8) >> 3);

            // Set Digits
            GPOS = ((CurrentSegment == TENS_SEGMENT || CurrentSegment == SC_SEGMENT) ? (1 << D8) : 0) | ((CurrentSegment == ONES_SEGMENT || CurrentSegment == SC_SEGMENT) ? (1 << D9) : 0);
            Phase = 1;
        }
        else
        {
            Phase = 0;
            ++CurrentSegment;
        }
    }
    else if (Phase == 0)
    {
        GPOC = (1 << D8) | (1 << D9) | (1 << D10);

        if (Colon_Flag == GAME_MINUTE && CurrentSegment == UPCOL_SEGMENT)
        {
            GPOS = (1 << D10);
        }
        else if (Colon_Flag >= GAME_SECONDS && CurrentSegment == DWNCOL_SEGMENT)
        {
            GPOS = (1 << D8) | (1 << D10);
        }
        else if (Timeout_Flag == HIGH && CurrentSegment == TOUT_SEGMENT)
        {
            GPOS = (1 << D8) | (1 << D9) | (1 << D10);
        }

        // Stage D3 now: no 4543 LD is high, and the delayed D input settles well before the next LD opens
        int TENS, ONES;
        GetDigits(CurrentSegment == TOUT_SEGMENT ? TENS_SEGMENT : CurrentSegment + 1, TENS, ONES);
        digitalWrite(D3, (TENS & 8) >> 3);

        Phase = 1;
    }
    else
    {
        Phase = 0;
        CurrentSegment = CurrentSegment == TOUT_SEGMENT ? TENS_SEGMENT : ++CurrentSegment;
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
    timer.attach_ms(LED_TICK_MS, [this]() { this->DisplayLED(); });
    interrupts();
}
