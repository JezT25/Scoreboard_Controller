/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"

bool WIFI_class::nc = true;

void WIFI_class::Initialize() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    esp_now_init();
    esp_now_register_recv_cb(OnDataRecv);

    while (nc) {
        Segment_1 = Segment_1 == 99 ? 0 : Segment_1 + 11;
        Segment_2 = Segment_1;
        Segment_3 = Segment_1;
        delay(1000);
    }
}

void WIFI_class::OnDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
    if (len == 0) return;

    char jsonStr[len + 1];

    memcpy(jsonStr, data, len); 
    jsonStr[len] = '\0';
    JsonDocument doc;
    deserializeJson(doc, jsonStr);

    Power_Flag = doc["PS"];
    Colon_Flag = doc["GD"];
    Timeout_Flag = doc["TF"];

    if (doc["CF"] == LOW)
    {
        Segment_1 = (Colon_Flag == GAME_SECONDS) ? doc["TS"] : doc["TM"];
        Segment_2 = (Colon_Flag == GAME_SECONDS) ? doc["TMS"].as<int>() * 10 : doc["TS"];
        Segment_3 = doc["SC"];
    }
    else
    {
        Segment_1 = doc["CH"];
        Segment_2 = doc["CM"];
        Segment_3 = TWO_DIGIT_DASH;
    }

    nc = false;
}
