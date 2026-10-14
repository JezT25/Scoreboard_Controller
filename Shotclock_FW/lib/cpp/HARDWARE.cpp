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

void IRAM_ATTR HARDWARE_class::ClearBus() {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, HIGH);
}

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

/*
    Each slot lasts 2 ticks. A 4543 latches the bus on its LD falling edge, and the slot is
    always closed with the bus blank (1111) so only the selected group is lit.
    D3 reaches the tens 4543 through the hardware delay, so for digits 0-7 (D3 LOW) it must go HIGH
    at least one delay time BEFORE LD falls, otherwise the latch captures 0111 (a stuck "7").

    Digit slot  tick 0: LD low -> write bus -> LD high (D3 was already staged in the previous colon slot)
                tick 1: hold, LD stays high (lit)
    Colon slot  tick 0: bus all HIGH while the group's LD is still high (delay window; D3 is already HIGH for 8, 9, blank)
                tick 1: LD low (latches blank) -> stage D3 for the next digit slot -> colon/timeout select
    LD is the BCD decoder output selected by D8-D10 (no dedicated pin); code 0 is the NC "transition" state.
*/
void IRAM_ATTR HARDWARE_class::DisplayLED() {
    if(Power_Flag != POWER_ON)
    {
        // Same blank-then-latch order as a colon slot so the last lit group latches blank
        if (Phase != 2) { ClearBus(); Phase = 2; }
        else GPOC = (1 << D8) | (1 << D9) | (1 << D10);
        return;
    }
    if (Phase == 2) Phase = 0;

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
        ClearBus();
        Phase = 1;
    }
    else
    {
        // LD falls with the bus (and delayed D3) already blank
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
