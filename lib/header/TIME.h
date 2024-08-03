/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

#ifndef TIME_h
#define TIME_h

class TIME_class : private HARDWARE_class {
    private:
        volatile unsigned int original_MIN = IData.TIME_MINUTE;
        volatile unsigned int original_SEC = IData.TIME_SECOND;
        volatile unsigned long prev_periodEnd;
        volatile unsigned long prev_SCEnd;
        volatile bool endPeriod = false;
        volatile bool endSC = false;

        void EndHander();
        void MainDisplayFunction();
        void ShotclockFunction();

    public:
        void SetRTC();
        void Function();  
        void TimeUpdate();  
};

#endif
