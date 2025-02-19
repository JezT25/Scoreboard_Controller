/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

void TIME_class::SetRTC() {
    rtc.setTime(IData.CLOCK_HOUR, IData.CLOCK_MINUTE, 0);
}

void TIME_class::EndHander() {
    if(endSC && millis() - prev_SCEnd >= BEEP_EX_LONG)
    {
        IData.SHOTCLOCK = (IData.TIME_MINUTE == 0 && IData.TIME_SECOND < 24) ? TWO_DIGIT_DASH : 24;
        endSC = false;
    }

    if(endPeriod && (millis() - prev_periodEnd >= BEEP_EXX_LONG || ISystem.TIME_MODE == TIME_RUNNING))
    {
        ISystem.TIME_MODE = TIME_PAUSE;
        IData.TIME_MINUTE = original_MIN;
        IData.TIME_SECOND = original_SEC;
        IData.TIME_MS = 0;
        IData.GAME_PERIOD = (IData.GAME_PERIOD == FOURTH_PERIOD) ? NO_PERIOD : (IData.GAME_PERIOD >> 1);
        IData.GAME_POSESSION = NO_POSESSION;
        IData.GAME_DOTS = GAME_MINUTE;
        endPeriod = false;

        IData.SHOTCLOCK = 24;
    }
}

void TIME_class::MainDisplayFunction() {
    if(IData.TIME_MINUTE == 0 && IData.TIME_SECOND < IData.SHOTCLOCK)
    {
        IData.SHOTCLOCK = TWO_DIGIT_DASH;
        ISystem.SC_TIME_MODE = TIME_RESET;
    }

    if(ISystem.TIME_MODE == TIME_ADJUST)
    {
        original_MIN = IData.TIME_MINUTE;
        original_SEC = IData.TIME_SECOND;
        IData.TIME_MS = 0;
    }
    else if(ISystem.TIME_MODE == TIME_RUNNING)
    {   
        if (IData.TIME_MS == 0)
        {
            if (IData.TIME_SECOND > 0)
            {
                if(IData.TIME_MINUTE == 0 && IData.TIME_SECOND <= 10)
                {
                    Beep(BEEP_SHORT, TONE_LOW);
                }
                IData.TIME_SECOND--;
            }
            else
            {
                if (IData.TIME_MINUTE > 0)
                {
                    IData.TIME_MINUTE--;
                    IData.TIME_SECOND = 59;
                }
                else
                {
                    IData.SHOTCLOCK = 0;
                    ISystem.TIME_MODE = TIME_PAUSE;
                    ISystem.SC_TIME_MODE = TIME_RESET;
                    Beep(BEEP_EXX_LONG, TONE_HIGH);
                    Honk(BEEP_EXX_LONG);
                    endPeriod = true;
                    endSC = false;
                    prev_periodEnd = millis();
                    return;
                }
            }
            IData.TIME_MS = 9;
        }
    }
}

void TIME_class::ShotclockFunction() {
    if(ISystem.SC_TIME_MODE == TIME_RESET)
    {
        IData.TIME_SC_MS = 9;
    }
    else if(ISystem.SC_TIME_MODE == TIME_RUNNING)
    {
        if (IData.SHOTCLOCK == 0)
        {
            ISystem.TIME_MODE = ISystem.TIME_MODE == TIME_RUNNING ? TIME_PAUSE : ISystem.TIME_MODE;
            ISystem.SC_TIME_MODE = TIME_RESET;
            Beep(BEEP_EX_LONG, TONE_HIGH);
            Honk(BEEP_EX_LONG);
            endSC = true;
            prev_SCEnd = millis();
            return;
        }
        else if (IData.TIME_SC_MS == 0)
        {
            if (IData.SHOTCLOCK > 0)
            {
                if(IData.SHOTCLOCK >= 2 && IData.SHOTCLOCK <= 5)
                {
                    Beep(BEEP_SHORT, TONE_LOW);
                }
                IData.SHOTCLOCK--;
            }
            IData.TIME_SC_MS = 9;
        }
    }
}

void TIME_class::Function() {
    EndHander();
    MainDisplayFunction();
    ShotclockFunction();
}

void TIME_class::TimeUpdate() {
    if(ISystem.TIME_MODE == TIME_RUNNING) IData.TIME_MS--;
    if(ISystem.SC_TIME_MODE == TIME_RUNNING) IData.TIME_SC_MS--;
    
    // Get RTC
    if(ISystem.TIME_MODE == TIME_CLOCKADJUST) return;
    IData.CLOCK_HOUR = (rtc.getTime().hour % 12 == 0) ? 12 : rtc.getTime().hour % 12;
    IData.CLOCK_MINUTE = rtc.getTime().min;
}
