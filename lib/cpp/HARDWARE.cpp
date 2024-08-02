/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

void HARDWARE_class::Honk(int duration) {
    digitalWrite(HORN, LOW);
    IHardware.honkTime = duration;
    IHardware.lastHonkTime = millis();
}

void HARDWARE_class::Beep(int duration, int frequency) {
    tone(BUZZER, frequency);
    IHardware.beepTime = duration;
    IHardware.lastBeepTime = millis();
}

void HARDWARE_class::DisableNoise() {
    if ((millis() - IHardware.lastBeepTime) >= IHardware.beepTime)
    {
        noTone(BUZZER);
    }
    if ((millis() - IHardware.lastHonkTime) >= IHardware.honkTime)
    {
        digitalWrite(HORN, HIGH);
    }
}

void HARDWARE_class::BUZZER_Initialize() {
    pinMode(BUZZER, OUTPUT);
}

void HARDWARE_class::BUTTON_Initialize() {
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
}

void HARDWARE_class::TIME_Initialize() {
    // Timer4 overflow interrupt ~ 10Hz
    TCCR4A = 0;
    TCCR4B = 0;
    OCR4A = 6249;
    TCCR4B |= (1 << WGM42);
    TCCR4B |= (1 << CS42) | (0 << CS41) | (0 << CS40);
    TIMSK4 |= (1 << OCIE4A);

    // Initialize RTC
	rtc.begin();
}

void HARDWARE_class::LED_Initialize() {
    // Set as OUTPUT
    for (int i = 0; i < 4; i++)
    {
        pinMode(LED_Section[i], OUTPUT);
        for (int j = 0; j < 7; j++)
        {
            pinMode(Segment_7[i][j], OUTPUT);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        pinMode(LED_Dots[i], OUTPUT);
    }

    // Timer5 overflow interrupt ~ 192Hz / 48Hz per section
    TCCR5A = 0;
    TCCR5B = 0;
    OCR5A = 10415;
    TCCR5B |= (1 << WGM52);
    TCCR5B |= (0 << CS52) | (1 << CS51) | (0 << CS50);
    TIMSK5 |= (1 << OCIE5A);
}

void HARDWARE_class::CheckForPower() {
    if (digitalRead(POWER_SW) == HIGH && ISystem.POWER_STATE != POWER_OFF)
    {
        ISystem.POWER_STATE = POWER_OFF;

        for (int segment = 0; segment < 4; segment++)
        {
            for (int j = 0; j < 7; j++)
            {
                digitalWrite(Segment_7[segment][j], LOW);
            }
        }
        for (int i = 0; i < 2; i++)
        {
            digitalWrite(LED_Dots[i], LOW);
        }
        for (int i = 0; i < 4; i++)
        {
            digitalWrite(LED_Section[i], LOW);
        }
        
        // digitalWrite(LED_Dots[1], HIGH);
        // digitalWrite(LED_Section[2], HIGH);
        // digitalWrite(LED_Section[3], HIGH);

        attachInterrupt(digitalPinToInterrupt(POWER_SW), [](){}, LOW);
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    }
    else if (digitalRead(POWER_SW) == LOW && ISystem.POWER_STATE != POWER_ON)
    {
        ISystem.POWER_STATE = POWER_ON;
        detachInterrupt(digitalPinToInterrupt(POWER_SW));
    }
}

void HARDWARE_class::Initialize() {
    noInterrupts();
    BUZZER_Initialize();
    BUTTON_Initialize();
    TIME_Initialize();
    LED_Initialize();
    interrupts();
}

void HARDWARE_class::Listener() {
    CheckForPower();
    DisableNoise();
}
