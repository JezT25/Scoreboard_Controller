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
            digitalWrite(D8, (CurrentSegment == TENS_SEGMENT || CurrentSegment == SC_SEGMENT) ? HIGH : LOW);
            digitalWrite(D9, (CurrentSegment == ONES_SEGMENT || CurrentSegment == SC_SEGMENT) ? HIGH : LOW);
            digitalWrite(D10, LOW);
            digitalWrite(D0, (TENS & 1));
            digitalWrite(D1, (TENS & 2) >> 1);
            digitalWrite(D2, (TENS & 4) >> 2);
            digitalWrite(D3, (TENS & 8) >> 3);
            digitalWrite(D4, (ONES & 1));
            digitalWrite(D5, (ONES & 2) >> 1);
            digitalWrite(D6, (ONES & 4) >> 2);
            digitalWrite(D7, (ONES & 8) >> 3);
        }
        else if (Colon_Flag == GAME_MINUTE && CurrentSegment == UPCOL_SEGMENT)
        {
            digitalWrite(D8, LOW);
            digitalWrite(D9, LOW);
            digitalWrite(D10, HIGH);
        }
        else if (Colon_Flag >= GAME_SECONDS && CurrentSegment == DWNCOL_SEGMENT)
        {
            digitalWrite(D8, HIGH);
            digitalWrite(D9, LOW);
            digitalWrite(D10, HIGH);
        }
        else if (Timeout_Flag == HIGH && CurrentSegment == TOUT_SEGMENT)
        {
            digitalWrite(D8, HIGH);
            digitalWrite(D9, HIGH);
            digitalWrite(D10, HIGH);
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
    timer.attach(LED_FREQ, [this]() { this->DisplayLED(); });
    interrupts();
}
