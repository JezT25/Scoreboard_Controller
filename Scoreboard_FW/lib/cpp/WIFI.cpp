/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

void WIFI_class::Initialize() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        Time_Minute = Time_Minute == 99 ? 0 : Time_Minute + 11;
        Time_Seconds = Time_Minute;
        Home_Score = Time_Minute;
        Away_Score = Time_Minute;
        Home_Fouls = Time_Minute / 10;
        Away_Fouls = Time_Minute / 10;
        Home_RTO = Time_Minute / 10;
        Away_RTO = Time_Minute / 10;
        Period = Time_Minute / 10;
        delay(CONNECTING_INTERVAL);
    }

    http.begin(client, SERVER);
}


void WIFI_class::GetUpdate() {
    // TODO: Add reconnecting algorithm!
    if (WiFi.status() == WL_CONNECTED && http.GET() > 0) {
        JsonDocument doc;
        deserializeJson(doc, http.getString());
        Period = doc["GAME_PERIOD"];
        Posession = doc["GAME_POSESSION"];
        Colon_Flag = doc["GAME_DOTS"];
        Time_Minute = (Colon_Flag == GAME_SECONDS) ? doc["TIME_SECOND"] : doc["TIME_MINUTE"];
        Time_Seconds = (Colon_Flag == GAME_SECONDS) ? doc["TIME_MS"].as<int>() * 10 : doc["TIME_SECOND"];
        Home_Score = doc["SCORE_HOME"];
        Away_Score = doc["SCORE_AWAY"];
        Home_Fouls = doc["FOUL_HOME"];
        Away_Fouls = doc["FOUL_AWAY"];
        Home_RTO = doc["TIMEOUT_HOME"];
        Away_RTO = doc["TIMEOUT_AWAY"];
    }

    delay(FETCH_INTERVAL);
}