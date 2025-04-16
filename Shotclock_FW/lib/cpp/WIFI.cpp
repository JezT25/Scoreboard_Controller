/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

void WIFI_class::Initialize() {
    if (analogRead(A0) < 900) WiFi.begin(WIFI_SSID1, WIFI_PASSWORD1);
    else WiFi.begin(WIFI_SSID2, WIFI_PASSWORD2);

    while (WiFi.status() != WL_CONNECTED) {
        Segment_1 = Segment_1 == 99 ? 0 : Segment_1 + 11;
        Segment_2 = Segment_1;
        Segment_3 = Segment_1;
        delay(CONNECTING_INTERVAL);
    }

    http.begin(client, SERVER);
}


void WIFI_class::GetUpdate() {
    // TODO: Add reconnecting algorithm!
    if (WiFi.status() == WL_CONNECTED && http.GET() > 0) {
        JsonDocument doc;
        deserializeJson(doc, http.getString());
        Power_Flag = doc["POWER_STATE"];
        Colon_Flag = doc["GAME_DOTS"];
        Timeout_Flag = doc["TIMEOUT_FLAG"];
        if(doc["CLOCK_FLAG"] == LOW)
        {
            Segment_1 = (Colon_Flag == GAME_SECONDS) ? doc["TIME_SECOND"] : doc["TIME_MINUTE"];
            Segment_2 = (Colon_Flag == GAME_SECONDS) ? doc["TIME_MS"].as<int>() * 10 : doc["TIME_SECOND"];
            Segment_3 = doc["SHOTCLOCK"];
        }
        else
        {
            Segment_1 = doc["CLOCK_HOUR"];
            Segment_2 = doc["CLOCK_MINUTE"];
            Segment_3 = DISABLE_DIGIT;
        }
    }
    delay(FETCH_INTERVAL);
}