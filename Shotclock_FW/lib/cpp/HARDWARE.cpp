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

// D3 rests HIGH by default. When the tens-digit data needs bit-3 LOW, we hold
// it LOW a little longer than a single write so the hardware-delayed 4028 "D"
// input reliably samples the LOW level, then release it back HIGH right away.
// When it needs HIGH, a brief LOW pulse gives the input a rising edge, since a steady HIGH is not registered.
void IRAM_ATTR HARDWARE_class::SetTensBit3(bool high) {
    if (high) {
        d3Timer.detach();
        digitalWrite(D3, LOW);
        delayMicroseconds(D3_HIGH_PULSE_US);
        digitalWrite(D3, HIGH);
    } else {
        digitalWrite(D3, LOW);
        d3Timer.once_ms(D3_LOW_HOLD_MS, []() { digitalWrite(D3, HIGH); });
    }
}

void IRAM_ATTR HARDWARE_class::DisplayLED() {
    // Only the digit-select lines are pre-cleared each tick (required for
    // multiplexing). The data bus (D0-D7) is no longer blanked; D3 rests HIGH
    // and is pulsed via SetTensBit3() instead, removing the whole-display blink.
    GPOC = (1 << D8) | (1 << D9) | (1 << D10);

    if(Power_Flag == POWER_ON)
    {
        if (CurrentSegment == TENS_SEGMENT || CurrentSegment == ONES_SEGMENT || CurrentSegment == SC_SEGMENT) {
            int TENS, ONES;

            if (CurrentSegment == SC_SEGMENT && Segment_3 == TWO_DIGIT_DASH)
            {
                TENS = DISABLE_DIGIT;
                ONES = DISABLE_DIGIT;
            }
            else
            {
                TENS = (CurrentSegment == TENS_SEGMENT ? Segment_1 : CurrentSegment == ONES_SEGMENT ? Segment_2 : Segment_3) / 10;
                ONES = (CurrentSegment == TENS_SEGMENT ? Segment_1 : CurrentSegment == ONES_SEGMENT ? Segment_2 : Segment_3) % 10;

                if(CurrentSegment == TENS_SEGMENT && TENS == 0) TENS = DISABLE_DIGIT;
                if(CurrentSegment == ONES_SEGMENT && Colon_Flag == GAME_SECONDS) ONES = DISABLE_DIGIT;
            }

            // Set Digits
            GPOS = ((CurrentSegment == TENS_SEGMENT || CurrentSegment == SC_SEGMENT) ? (1 << D8) : 0) | ((CurrentSegment == ONES_SEGMENT || CurrentSegment == SC_SEGMENT) ? (1 << D9) : 0);
            digitalWrite(D0, (TENS & 1));
            digitalWrite(D1, (TENS & 2) >> 1);
            digitalWrite(D2, (TENS & 4) >> 2);
            SetTensBit3((TENS & 8) >> 3);
            digitalWrite(D4, (ONES & 1));
            digitalWrite(D5, (ONES & 2) >> 1);
            digitalWrite(D6, (ONES & 4) >> 2);
            digitalWrite(D7, (ONES & 8) >> 3);
        }
        else if (Colon_Flag == GAME_MINUTE && CurrentSegment == UPCOL_SEGMENT)
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
    digitalWrite(D3, HIGH); // D3 resting state, see SetTensBit3()
    timer.attach(LED_FREQ, [this]() { this->DisplayLED(); });
    interrupts();
}
