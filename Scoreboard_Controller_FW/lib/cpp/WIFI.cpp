/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

const char* IDATA::toJSON() {
    static char buffer[350]; // Allocate just enough buffer space
	
    int length = snprintf(
        buffer, sizeof(buffer),
        "{\"SH\":%d,\"FH\":%d,\"TH\":%d,"
        "\"SA\":%d,\"FA\":%d,\"TA\":%d,"
        "\"SC\":%d,\"TM\":%d,\"TS\":%d,"
        "\"TMS\":%d,\"CH\":%d,\"CM\":%d,"
        "\"GP\":%d,\"GS\":%d,\"GD\":%d,"
        "\"TF\":%d,\"CF\":%d,\"PS\":%d}",
        SCORE_HOME,
        FOUL_HOME,
        TIMEOUT_HOME,
        SCORE_AWAY,
        FOUL_AWAY,
        TIMEOUT_AWAY,
        SHOTCLOCK,
        TIME_MINUTE,
        TIME_SECOND,
        TIME_MS,
        CLOCK_HOUR,
        CLOCK_MINUTE,
        GAME_PERIOD,
        GAME_POSESSION,
        GAME_DOTS,
        TIMEOUT_FLAG,
        CLOCK_FLAG,
        ISystem.POWER_STATE
    );

    if (length < 0 || length >= sizeof(buffer)) {
        return "{}"; // Fallback in case of error
    }

    return buffer;
}


void WIFI_class::SendUpdate() {
    if(ISystem.POWER_STATE != POWER_ON) return;

	if(millis() - lastWiFiUpdate >= WIFI_INTERVAL) {
		lastWiFiUpdate = millis();
		Serial3.println(IData.toJSON());
	}
}