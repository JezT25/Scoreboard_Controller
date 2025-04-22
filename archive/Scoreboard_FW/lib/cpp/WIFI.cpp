/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

bool WIFI_class::coldboot = true;

void WIFI_class::Initialize() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    esp_now_init();
    esp_now_register_recv_cb(OnDataRecv);

    while (coldboot) {
        Period = (Period == 8) ? 4 : (Period == 4) ? 2 : (Period == 2) ? 1 : 8;
        Time_Minute = Time_Minute == 99 ? 0 : Time_Minute + 11;
        Time_Seconds = Time_Minute;
        Home_Score = Time_Minute;
        Away_Score = Time_Minute;
        Home_Fouls = Time_Minute / 10;
        Away_Fouls = Time_Minute / 10;
        Home_RTO = Time_Minute / 10;
        Away_RTO = Time_Minute / 10;
        delay(1000);
    }
}

void WIFI_class::OnDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
    if(len == 0) return;

    char jsonStr[len + 1];

    memcpy(jsonStr, data, len); 
    jsonStr[len] = '\0';
    JsonDocument doc;
    deserializeJson(doc, jsonStr);

    Power_Flag = doc["PS"];
    Colon_Flag = doc["GD"];
    Period = doc["GP"];
    Posession = doc["GS"];
    Home_Score = doc["SH"];
    Away_Score = doc["SA"];
    Home_Fouls = doc["FH"];
    Away_Fouls = doc["FA"];
    Home_RTO = doc["TH"];
    Away_RTO = doc["TA"];
    Clock_Flag = doc["CF"];

    if(Clock_Flag == LOW)
    {
        Time_Minute = (Colon_Flag == GAME_SECONDS) ? doc["TS"] : doc["TM"];
        Time_Seconds = (Colon_Flag == GAME_SECONDS) ? doc["TMS"].as<int>() * 10 : doc["TS"];
    }
    else
    {
        Time_Minute = doc["CH"];
        Time_Seconds = doc["CM"];
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