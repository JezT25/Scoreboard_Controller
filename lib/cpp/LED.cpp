/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

void LED_class::UpdateMiniBuffer() {
    for (int i = 0; i < 4; i++)
    {
        Dots_Buffer[0][i] = (IData.GAME_PERIOD >> (3 - i)) & 1;
        Dots_Buffer[1][i] = ((IData.GAME_POSESSION | IData.GAME_DOTS) >> (3 - i)) & 1;
    }
}

void LED_class::UpdateBuffer(int number, int location) {
    for (int i = 0; i < 7; i++)
    {
        Segment_Buffer[location % 10][location / 10][i] = bitRead(LED_digitValues[number], i);
    }
}

void LED_class::ClearBuffer() {
    UpdateMiniBuffer();
    for (int i = FOUL_HOME; i <= TIME_ONES; i++)
    {
        i = (i == TIMEOUT_AWAY + 1) ? SHOTCLOCK_TENS : ((i == SHOTCLOCK_ONES + 1) ? SCORE_HOME_TENS : ((i == SCORE_AWAY_ONES + 1) ? TIME_MIN_TENS : i));
        UpdateBuffer(0, i);
    }
    coldBoot = false;
}

void LED_class::RefreshBuffer() {
    // From dead initialization never to be rerun again
    coldBoot ? ClearBuffer() : static_cast<void>(0);

    if(ISystem.TIME_MODE == TIME_ADJUST && millis() - lastBlinkTime >= BLINK_INTERVAL)
    {
        if(BlinkState)
        {
            UpdateBuffer(10, TIME_MIN_TENS);
            UpdateBuffer(10, TIME_MIN_ONES);
            UpdateBuffer(10, TIME_TENS);
            UpdateBuffer(10, TIME_ONES);
            IData.GAME_DOTS = GAME_HIDE;
        }
        else
        {
            UpdateBuffer(IData.TIME_MINUTE / 10, TIME_MIN_TENS);
            UpdateBuffer(IData.TIME_MINUTE % 10, TIME_MIN_ONES);
            UpdateBuffer(IData.TIME_SECOND / 10, TIME_TENS);
            UpdateBuffer(IData.TIME_SECOND % 10, TIME_ONES);
            IData.GAME_DOTS = GAME_MINUTE;
        }
        UpdateMiniBuffer();
        BlinkState = !BlinkState;
        lastBlinkTime = millis();
    }
    else if(!BlinkState && ISystem.TIME_MODE != TIME_ADJUST)
    {
        UpdateBuffer(IData.TIME_MINUTE / 10, TIME_MIN_TENS);
        UpdateBuffer(IData.TIME_MINUTE % 10, TIME_MIN_ONES);
        UpdateBuffer(IData.TIME_SECOND / 10, TIME_TENS);
        UpdateBuffer(IData.TIME_SECOND % 10, TIME_ONES);
        BlinkState = !BlinkState;
        IData.GAME_DOTS = GAME_MINUTE;
        UpdateMiniBuffer();
    }

    if(ISystem.TIME_MODE != TIME_ADJUST && ISystem.TIME_DIRECTION == TIME_DIRECTION_DOWN && IData.TIME_MINUTE == 0)
    {
        UpdateBuffer(IData.TIME_SECOND / 10 == 0 ? 10 : IData.TIME_SECOND / 10, TIME_MIN_TENS);
        UpdateBuffer(IData.TIME_SECOND % 10, TIME_MIN_ONES);
        UpdateBuffer(IData.TIME_MS, TIME_TENS);
        UpdateBuffer(10, TIME_ONES);
        IData.GAME_DOTS = GAME_SECONDS;

        if(IData.TIME_MINUTE == 0 && IData.TIME_SECOND == 0 && IData.TIME_MS == 0)
        {
            pTime_Minute = IData.TIME_MINUTE;
            pTime_Second = IData.TIME_SECOND;
        }
    }
    else
    {   
        if(IData.TIME_MINUTE != pTime_Minute)
        {
            UpdateBuffer(IData.TIME_MINUTE / 10, TIME_MIN_TENS);
            UpdateBuffer(IData.TIME_MINUTE % 10, TIME_MIN_ONES);
            pTime_Minute = IData.TIME_MINUTE;
        }
        if(IData.TIME_SECOND != pTime_Second)
        {
            UpdateBuffer(IData.TIME_SECOND / 10, TIME_TENS);
            UpdateBuffer(IData.TIME_SECOND % 10, TIME_ONES);
            pTime_Second = IData.TIME_SECOND;
        }
    }
    if(IData.SCORE_HOME != pScore_Home)
    {
        UpdateBuffer(IData.SCORE_HOME / 10, SCORE_HOME_TENS);
        UpdateBuffer(IData.SCORE_HOME % 10, SCORE_HOME_ONES);
        pScore_Home = IData.SCORE_HOME;
    }
    if(IData.FOUL_HOME != pFoul_Home)
    {
        UpdateBuffer(IData.FOUL_HOME, FOUL_HOME);
        pFoul_Home = IData.FOUL_HOME;
    }
    if(IData.TIMEOUT_HOME != pTimeout_Home)
    {
        UpdateBuffer(IData.TIMEOUT_HOME, TIMEOUT_HOME);
        pTimeout_Home = IData.TIMEOUT_HOME;
    }
    if(IData.SCORE_AWAY != pScore_Away)
    {
        UpdateBuffer(IData.SCORE_AWAY / 10, SCORE_AWAY_TENS);
        UpdateBuffer(IData.SCORE_AWAY % 10, SCORE_AWAY_ONES);
        pScore_Away = IData.SCORE_AWAY;
    }
    if(IData.FOUL_AWAY != pFoul_Away)
    {
        UpdateBuffer(IData.FOUL_AWAY, FOUL_AWAY);
        pFoul_Away = IData.FOUL_AWAY;
    }
    if(IData.TIMEOUT_AWAY != pTimeout_Away)
    {
        UpdateBuffer(IData.TIMEOUT_AWAY, TIMEOUT_AWAY);
        pTimeout_Away = IData.TIMEOUT_AWAY;
    }
    if(IData.SHOTCLOCK != pShotclock)
    {
        if(IData.SHOTCLOCK == TWO_DIGIT_DASH || IData.SHOTCLOCK == TWO_DIGIT_OFF)
        {
            UpdateBuffer(IData.SHOTCLOCK / 10, SHOTCLOCK_TENS);
            UpdateBuffer(IData.SHOTCLOCK / 10, SHOTCLOCK_ONES);
        }
        else
        {
            UpdateBuffer(IData.SHOTCLOCK / 10, SHOTCLOCK_TENS);
            UpdateBuffer(IData.SHOTCLOCK % 10, SHOTCLOCK_ONES);
        }
        pShotclock = IData.SHOTCLOCK;
    }
    if (IData.GAME_PERIOD != pPeriod || IData.GAME_POSESSION != pPosession || IData.GAME_DOTS != pDots) {
        UpdateMiniBuffer();
        pPeriod = IData.GAME_PERIOD;
        pPosession = IData.GAME_POSESSION;
        pDots = IData.GAME_DOTS;
    }
}

void LED_class::DisplayDigits() {
    if(ISystem.POWER_STATE == POWER_OFF)
    {
        for (int segment = 0; segment < 4; segment++)
        {
            for (int j = 0; j < 7; j++) {
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
        
        digitalWrite(LED_Dots[1], HIGH);
        digitalWrite(LED_Section[2], HIGH);
        digitalWrite(LED_Section[3], HIGH);

        ISystem.POWER_STATE = POWER_IDLE;
    }
    else if(ISystem.POWER_STATE == POWER_ON)
    {
        digitalWrite(LED_Section[currentSegment], LOW);
        currentSegment = (currentSegment + 1) % 4;
        for (int segment = 0; segment < 4; segment++)
        {
            for (int j = 0; j < 7; j++)
            {
                digitalWrite(Segment_7[segment][j], Segment_Buffer[currentSegment][segment][j]);
            }
        }
        for (int i = 0; i < 2; i++)
        {
            digitalWrite(LED_Dots[i], Dots_Buffer[i][currentSegment]);
        }
        digitalWrite(LED_Section[currentSegment], HIGH);
    }
}