/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

String IDATA::toJSON() {
    static char buffer[350]; // Allocate just enough buffer space
	
    int length = snprintf(
        buffer, sizeof(buffer),
        "{\"SCORE_HOME\":%d,\"FOUL_HOME\":%d,\"TIMEOUT_HOME\":%d,"
        "\"SCORE_AWAY\":%d,\"FOUL_AWAY\":%d,\"TIMEOUT_AWAY\":%d,"
        "\"SHOTCLOCK\":%d,\"TIME_MINUTE\":%d,\"TIME_SECOND\":%d,"
        "\"TIME_MS\":%d,\"TIME_SC_MS\":%d,\"CLOCK_HOUR\":%d,"
        "\"CLOCK_MINUTE\":%d,\"GAME_PERIOD\":%d,\"GAME_POSESSION\":%d,"
        "\"GAME_DOTS\":%d}",
        SCORE_HOME, FOUL_HOME, TIMEOUT_HOME, SCORE_AWAY, FOUL_AWAY, TIMEOUT_AWAY,
        SHOTCLOCK, TIME_MINUTE, TIME_SECOND, TIME_MS, TIME_SC_MS, CLOCK_HOUR,
        CLOCK_MINUTE, GAME_PERIOD, GAME_POSESSION, GAME_DOTS
    );

    if (length < 0 || length >= sizeof(buffer)) {
        return "{}"; // Fallback in case of error
    }

    return String(buffer);
}


void WIFI_class::SendUpdate() {
	if(millis() - lastWiFiUpdate >= WIFI_INTERVAL) {
		lastWiFiUpdate = millis();
		Serial3.println(IData.toJSON());
	}
}