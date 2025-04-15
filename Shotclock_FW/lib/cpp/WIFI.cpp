/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

void WIFI_class::Initialize() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

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
        Colon_Flag = doc["GAME_DOTS"];
        Timeout_Flag = doc["TIMEOUT_FLAG"];
        Segment_1 = (Colon_Flag == GAME_SECONDS) ? doc["TIME_SECOND"] : doc["TIME_MINUTE"];
        Segment_2 = (Colon_Flag == GAME_SECONDS) ? doc["TIME_MS"].as<int>() * 10 : doc["TIME_SECOND"];
        Segment_3 = doc["SHOTCLOCK"];
    }

    delay(FETCH_INTERVAL);
}