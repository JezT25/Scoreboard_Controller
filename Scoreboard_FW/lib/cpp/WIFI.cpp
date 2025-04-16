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
        Period = (Period == 8) ? 4 : (Period == 4) ? 2 : (Period == 2) ? 1 : 8;
        Time_Minute = Time_Minute == 99 ? 0 : Time_Minute + 11;
        Time_Seconds = Time_Minute;
        Home_Score = Time_Minute;
        Away_Score = Time_Minute;
        Home_Fouls = Time_Minute / 10;
        Away_Fouls = Time_Minute / 10;
        Home_RTO = Time_Minute / 10;
        Away_RTO = Time_Minute / 10;
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
        Period = doc["GAME_PERIOD"];
        Posession = doc["GAME_POSESSION"];
        Home_Score = doc["SCORE_HOME"];
        Away_Score = doc["SCORE_AWAY"];
        Home_Fouls = doc["FOUL_HOME"];
        Away_Fouls = doc["FOUL_AWAY"];
        Home_RTO = doc["TIMEOUT_HOME"];
        Away_RTO = doc["TIMEOUT_AWAY"];
        Clock_Flag = doc["CLOCK_FLAG"];

        if(Clock_Flag == LOW)
        {
            Time_Minute = (Colon_Flag == GAME_SECONDS) ? doc["TIME_SECOND"] : doc["TIME_MINUTE"];
            Time_Seconds = (Colon_Flag == GAME_SECONDS) ? doc["TIME_MS"].as<int>() * 10 : doc["TIME_SECOND"];
        }
        else
        {
            Time_Minute = doc["CLOCK_HOUR"];
            Time_Seconds = doc["CLOCK_MINUTE"];
        }

        if(Clock_Flag == LOW || coldboot == true)
        {
            pPeriod = Period;
            pPosession = Posession;
            pHome_Score = Home_Score;
            pAway_Score = Away_Score;
            pHome_Fouls = Home_Fouls;
            pAway_Fouls = Away_Fouls;
            pHome_RTO = Home_RTO;
            pAway_RTO = Away_RTO;
            coldboot = false;
        }
    }
    delay(FETCH_INTERVAL);
}