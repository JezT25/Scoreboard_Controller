/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#ifndef BUTTON_h
#define BUTTON_h

#include "../setup.hpp"

#define DEBOUNCE_NONE   0
#define DEBOUNCE_SHORT  50
#define DEBOUNCE_MED    100
#define DEBOUNCE_LONG   200
#define HOLD_INTERVAL   100
#define HOLD_TIME       1500

#define BUTTON_DEBOUNCE_COUNT   13

class BUTTON_class : private HARDWARE_class {
    private:
        bool SCHeldWhileRunning = false;
        bool TimeOnOFFPressed   = false;
        unsigned int original_MIN = IData.TIME_MINUTE;
        unsigned int original_SEC = IData.TIME_SECOND;
        unsigned int debounceOffset;
        unsigned long lastTimeButtonTime;
        unsigned long lastLongDebounceTime;
        unsigned long lastHoldDelayTime[BUTTON_DEBOUNCE_COUNT]  = { 0 };
        unsigned long lastDebounceTime[BUTTON_DEBOUNCE_COUNT]   = { 0 };
        unsigned long lastHoldTime[BUTTON_DEBOUNCE_COUNT]       = { 0 };
        int lastButtonState[BUTTON_DEBOUNCE_COUNT]              = { HIGH };
        bool buttonWasPressed[BUTTON_DEBOUNCE_COUNT]            = { false };

        void ButtonReleaseFunctions(int i);
        void ButtonFunctions(int i, bool holdButton);

    public:
        void Function();
};

#endif
