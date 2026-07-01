/*******************************************
    DEVELOPED BY JEZREEL TAN - DEC 2023
    jztan25@gmail.com
    (0917) 443 2532
*******************************************/
#include "../setup.hpp"

void TIME_class::SetRTC()
{
    rtc.setTime(IData.CLOCK_HOUR, IData.CLOCK_MINUTE, 0);
}

void TIME_class::EndHander()
{
    if (endSC && millis() - prev_SCEnd >= BEEP_EX_LONG)
    {
        // Shot clock alert complete - clear flag
        // SHOTCLOCK already reset to 24 and running
        IData.TIMEOUT_FLAG = LOW;
        endSC = false;
    }

    if (endPeriod && millis() - prev_periodEnd >= BEEP_EXX_LONG)
    {
        IData.PERIOD_END_FLAG = HIGH;
        ISystem.TIME_MODE = TIME_PAUSE;
        IData.TIME_MS = 0;
        // Foul reset is now synchronized with Time On/Off button press (period advancement)
        // Do not reset fouls here - they will be reset in ButtonReleaseFunctions() when TIME_BUTTON is pressed

        endPeriod = false;
    }
}

void TIME_class::MainDisplayFunction()
{
    // Only show dash if shot clock is not running/expired and game time is less than shot clock display
    if (ISystem.SC_TIME_MODE != TIME_RUNNING && ISystem.SC_TIME_MODE != TIME_EXPIRED && IData.TIME_MINUTE == 0 && IData.TIME_SECOND < IData.SHOTCLOCK)
    {
        IData.SHOTCLOCK = TWO_DIGIT_DASH;
        ISystem.SC_TIME_MODE = TIME_RESET;
    }

    if (ISystem.TIME_MODE == TIME_ADJUST)
    {
        IData.TIME_MS = 0;
    }
    else if (ISystem.TIME_MODE == TIME_RUNNING)
    {
        if (IData.TIME_MS == 0)
        {
            if (IData.TIME_SECOND > 0)
            {
                if (IData.TIME_MINUTE == 0 && IData.TIME_SECOND <= 10)
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
                    IData.TIMEOUT_FLAG = HIGH;
                    IData.SHOTCLOCK = TWO_DIGIT_DASH;
                    ISystem.TIME_MODE = TIME_STOP;
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

void TIME_class::ShotclockFunction()
{
    if (ISystem.SC_TIME_MODE == TIME_RESET)
    {
        IData.TIME_SC_MS = 9;
    }
    else if (ISystem.SC_TIME_MODE == TIME_RUNNING)
    {
        if (IData.SHOTCLOCK == 0)
        {
            // Shot clock expired - freeze at 0 with series light on
            // Game clock continues uninterrupted
            ISystem.SC_TIME_MODE = TIME_EXPIRED;
            IData.TIMEOUT_FLAG = HIGH;
            Beep(BEEP_EX_LONG, TONE_HIGH);
            Honk(BEEP_EX_LONG);
            endSC = true;
            prev_SCEnd = millis();
            return;
        }
        else if (IData.TIME_SC_MS == 0)
        {
            // Only decrement normal SHOTCLOCK values (0-24), not special values (>= 100)
            if (IData.SHOTCLOCK > 0 && IData.SHOTCLOCK < 100)
            {
                if (IData.SHOTCLOCK >= 2 && IData.SHOTCLOCK <= 5)
                {
                    Beep(BEEP_SHORT, TONE_LOW);
                }
                IData.SHOTCLOCK--;
            }
            IData.TIME_SC_MS = 9;
        }
    }
    else if (ISystem.SC_TIME_MODE == TIME_EXPIRED)
    {
        // Shotclock is frozen at 0 - only decrement main display ms if needed
        IData.TIME_SC_MS = 9;
        // Keep SHOTCLOCK at 0 and TIMEOUT_FLAG HIGH (series light on)
    }
}

void TIME_class::Function()
{
    EndHander();
    MainDisplayFunction();
    ShotclockFunction();
}

void TIME_class::TimeUpdate()
{
    if (ISystem.TIME_MODE == TIME_RUNNING)
        IData.TIME_MS--;
    if (ISystem.SC_TIME_MODE == TIME_RUNNING)
        IData.TIME_SC_MS--;

    // Get RTC
    if (ISystem.TIME_MODE == TIME_CLOCKADJUST)
        return;
    IData.CLOCK_HOUR = (rtc.getTime().hour % 12 == 0) ? 12 : rtc.getTime().hour % 12;
    IData.CLOCK_MINUTE = rtc.getTime().min;
}
