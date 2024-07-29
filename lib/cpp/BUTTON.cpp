/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

void BUTTON_class::ButtonReleaseFunctions(int i) {
    const bool isTimeMinPressed  = digitalRead(TIME_MIN);
    const bool isTimeSecPressed  = digitalRead(TIME_SEC);

    if (((buttonPins[i] == TIME_MIN) && buttonWasPressed[i] && buttonWasPressed[i + 1]) || ((buttonPins[i] == TIME_SEC) && buttonWasPressed[i] && buttonWasPressed[i - 1]))
    {
        if (isTimeSecPressed && isTimeMinPressed && ((millis() - lastLongDebounceTime) >= (DEBOUNCE_SHORT + debounceOffset)))
        {
            IData.GAME_PERIOD = (IData.GAME_PERIOD == FOURTH_PERIOD) ? NO_PERIOD : (IData.GAME_PERIOD >> 1);
            Beep(BEEP_SHORT, TONE_HIGH);
            debounceOffset = DEBOUNCE_NONE;
        }
        lastLongDebounceTime = millis();
    }
    else if(buttonPins[i] == TIME_BUTTON && TimeOnOFFPressed)
    {
        if (millis() - lastTimeButtonTime >= DEBOUNCE_SHORT && TimeOnOFFPressed)
        {
            ISystem.TIME_MODE = ISystem.TIME_MODE == TIME_RUNNING ? TIME_PAUSE : TIME_RUNNING;
            ISystem.SC_TIME_MODE = TIME_PAUSE;
            Beep(BEEP_SHORT, TONE_HIGH);
        }
        TimeOnOFFPressed = false;
    }
    else if(buttonPins[i] == SC_PRESET && SCHeldWhileRunning)
    {
        ISystem.SC_TIME_MODE = TIME_RUNNING;
        SCHeldWhileRunning = false;
    }
}

void BUTTON_class::ButtonFunctions(int i, bool holdButton = false) {
    const bool isHomeUpPressed   = digitalRead(HOME_UP);
    const bool isHomeDownPressed = digitalRead(HOME_DOWN);
    const bool isAwayUpPressed   = digitalRead(AWAY_UP);
    const bool isAwayDownPressed = digitalRead(AWAY_DOWN);
    const bool isTimeMinPressed  = digitalRead(TIME_MIN);
    const bool isTimeSecPressed  = digitalRead(TIME_SEC);
    const bool isSCPresetPressed = digitalRead(SC_PRESET);
    const bool isSCOnOffPressed  = digitalRead(SC_STARTSTOP);

    if (((buttonPins[i] == HOME_UP && !isHomeDownPressed) || (buttonPins[i] == HOME_DOWN && !isHomeUpPressed)))
    {
        if(!holdButton)
        {
            if(ISystem.TIME_MODE == TIME_ADJUST)
            {
                IData.TIME_MINUTE = 0;
            }
            else
            {
                IData.SCORE_HOME = 0;
            }
            Beep(BEEP_MED, TONE_HIGH);
        }
    }
    else if (((buttonPins[i] == AWAY_UP && !isAwayDownPressed) || (buttonPins[i] == AWAY_DOWN && !isAwayUpPressed)))
    {
        if(!holdButton)
        {
            if(ISystem.TIME_MODE == TIME_ADJUST)
            {
                IData.TIME_SECOND = 0;
            }
            else
            {
                IData.SCORE_AWAY = 0;
            }
            Beep(BEEP_MED, TONE_HIGH);
        }
    }
    else if ((buttonPins[i] == TIME_MIN && !isTimeSecPressed) || (buttonPins[i] == TIME_SEC && !isTimeMinPressed))
    {
        if(holdButton && (millis() - lastLongDebounceTime) >= DEBOUNCE_LONG)
        {
            if(ISystem.TIME_MODE == TIME_RUNNING) return;
            ISystem.TIME_MODE = ISystem.TIME_MODE == TIME_PAUSE ? TIME_ADJUST : TIME_PAUSE;
            Beep(BEEP_LONG, TONE_HIGH);
            debounceOffset = DEBOUNCE_MED;
        }
        lastLongDebounceTime = millis();
    }
    else
    {
        switch (buttonPins[i])
        {
            case HOME_UP:
                if(ISystem.TIME_MODE == TIME_ADJUST)
                {
                    IData.TIME_MINUTE == 23 ? IData.TIME_MINUTE = 0 : IData.TIME_MINUTE++;
                }
                else
                {
                    IData.SCORE_HOME == 99 ? IData.SCORE_HOME = 0 : IData.SCORE_HOME++;
                }
                Beep(BEEP_SHORT, TONE_HIGH);
                break;
            case HOME_DOWN:
                if(ISystem.TIME_MODE == TIME_ADJUST)
                {
                    IData.TIME_MINUTE == 0 ? IData.TIME_MINUTE = 23 : IData.TIME_MINUTE--;
                }
                else
                {
                    IData.SCORE_HOME == 0 ? IData.SCORE_HOME = 99 : IData.SCORE_HOME--;
                }
                Beep(BEEP_SHORT, TONE_LOW);
                break;
            case AWAY_UP:
                if(ISystem.TIME_MODE == TIME_ADJUST)
                {
                    IData.TIME_SECOND == 59 ? IData.TIME_SECOND = 0 : IData.TIME_SECOND++;
                }
                else
                {
                    IData.SCORE_AWAY == 99 ? IData.SCORE_AWAY = 0 : IData.SCORE_AWAY++;
                }
                Beep(BEEP_SHORT, TONE_HIGH);
                break;
            case AWAY_DOWN:
                if(ISystem.TIME_MODE == TIME_ADJUST)
                {
                    IData.TIME_SECOND == 0 ? IData.TIME_SECOND = 59 : IData.TIME_SECOND--;
                }
                else
                {
                    IData.SCORE_AWAY == 0 ? IData.SCORE_AWAY = 99 : IData.SCORE_AWAY--;
                }
                Beep(BEEP_SHORT, TONE_LOW);
                break;
            case HOME_FOUL:
                IData.FOUL_HOME == 9 ? IData.FOUL_HOME = 0 : IData.FOUL_HOME++;
                Beep(BEEP_SHORT, TONE_HIGH);
                break;
            case HOME_TIMEOUT:
                IData.TIMEOUT_HOME == 0 ? IData.TIMEOUT_HOME = 4 : IData.TIMEOUT_HOME--;
                Beep(BEEP_SHORT, TONE_LOW);
                break;
            case AWAY_FOUL:
                IData.FOUL_AWAY == 9 ? IData.FOUL_AWAY = 0 : IData.FOUL_AWAY++;
                Beep(BEEP_SHORT, TONE_HIGH);
                break;
            case AWAY_TIMEOUT:
                IData.TIMEOUT_AWAY == 0 ? IData.TIMEOUT_AWAY = 4 : IData.TIMEOUT_AWAY--;
                Beep(BEEP_SHORT, TONE_LOW);
                break;
            case TIME_MIN:
                if(!holdButton)
                {
                    IData.GAME_POSESSION = IData.GAME_POSESSION == HOME_POSESSION ? NO_POSESSION : HOME_POSESSION;
                    Beep(BEEP_SHORT, TONE_HIGH);
                }
                break;
            case TIME_SEC:
                if(!holdButton)
                {
                    IData.GAME_POSESSION = IData.GAME_POSESSION == AWAY_POSESSION ? NO_POSESSION : AWAY_POSESSION;
                    Beep(BEEP_SHORT, TONE_HIGH);
                }
                break;
            case TIME_BUTTON:
                if(ISystem.TIME_MODE == TIME_ADJUST) return;
                if(!holdButton && !TimeOnOFFPressed) {
                    TimeOnOFFPressed = true;
                    lastTimeButtonTime = millis();
                }
                else if(holdButton && ISystem.TIME_MODE == TIME_PAUSE && TimeOnOFFPressed)
                {
                    ISystem.TIME_DIRECTION = ISystem.TIME_DIRECTION == TIME_DIRECTION_UP ? TIME_DIRECTION_DOWN : TIME_DIRECTION_UP;
                    IData.TIME_MS = 0;
                    Beep(BEEP_MED, TONE_HIGH);
                    TimeOnOFFPressed = false;
                }
                break;
            case SC_PRESET:
                if(!isSCOnOffPressed || ISystem.TIME_MODE == TIME_ADJUST || ISystem.TIME_DIRECTION == TIME_DIRECTION_UP) return;
                if(!holdButton)
                {   
                    if(IData.TIME_MINUTE == 0 && IData.TIME_SECOND < 24)
                    {
                        IData.SHOTCLOCK = IData.TIME_SECOND < 14 ? IData.SHOTCLOCK = TWO_DIGIT_DASH : 14;
                    }
                    else if(ISystem.SC_TIME_MODE != TIME_RUNNING)
                    {
                        IData.SHOTCLOCK = IData.SHOTCLOCK == 24 ? 14 : 24;
                    }
                    else if(IData.SHOTCLOCK == 24 && IData.TIME_SC_MS > 7)
                    {
                        IData.SHOTCLOCK = 14;
                    }
                    else if(IData.SHOTCLOCK != 24)
                    {
                        IData.SHOTCLOCK = 24;
                    }
                    Beep(BEEP_SHORT, TONE_HIGH);
                }
                if(ISystem.SC_TIME_MODE == TIME_RUNNING) SCHeldWhileRunning = true;
                IData.TIME_SC_MS = 9;
                ISystem.SC_TIME_MODE = TIME_RESET;
                break;
            case SC_STARTSTOP:
                if(!isSCPresetPressed || IData.SHOTCLOCK == TWO_DIGIT_DASH || ISystem.TIME_MODE == TIME_ADJUST || ISystem.TIME_DIRECTION == TIME_DIRECTION_UP) return;
                if(holdButton || IData.SHOTCLOCK == TWO_DIGIT_DASH)
                {
                    ISystem.SC_TIME_MODE = TIME_RESET;
                    if(holdButton && IData.SHOTCLOCK != TWO_DIGIT_DASH)
                    {
                        IData.SHOTCLOCK = TWO_DIGIT_DASH;
                        Beep(BEEP_MED, TONE_HIGH);
                    }
                    else if (!holdButton && IData.SHOTCLOCK == TWO_DIGIT_DASH)
                    {
                        IData.SHOTCLOCK = 24;
                        Beep(BEEP_MED, TONE_HIGH);
                    }
                    return;
                }
                if(ISystem.TIME_MODE != TIME_RUNNING) return;
                ISystem.SC_TIME_MODE = ISystem.SC_TIME_MODE == TIME_RUNNING ? TIME_PAUSE : TIME_RUNNING;
                Beep(BEEP_SHORT, TONE_HIGH);
                break;
        }
    }
}

void BUTTON_class::Function() {
    if(ISystem.POWER_STATE != POWER_ON) return;

    for (int i = 0; i < BUTTON_DEBOUNCE_COUNT; i++)
    {
        int buttonReading = digitalRead(buttonPins[i]);

        if (buttonReading != lastButtonState[i])
        {
            lastDebounceTime[i] = millis();
        }

        if ((millis() - lastDebounceTime[i]) >= DEBOUNCE_SHORT)
        {
            if (buttonReading == LOW)
            {
                if(buttonWasPressed[i])
                {
                    if((millis() - lastHoldTime[i] >= HOLD_TIME) && (millis() - lastHoldDelayTime[i] >= HOLD_INTERVAL))
                    {
                        ButtonFunctions(i, true);
                        lastHoldDelayTime[i] = millis();
                    }
                }
                else if(!buttonWasPressed[i])
                {
                    ButtonFunctions(i);
                    buttonWasPressed[i] = true;
                    lastHoldTime[i] = millis();
                }
            }
            else if (buttonReading == HIGH)
            {
                ButtonReleaseFunctions(i);
                buttonWasPressed[i] = false;
            }
        }

        lastButtonState[i] = buttonReading;
    }
}
