/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/
#include "../setup.hpp"
#include <EEPROM.h>
#include <user_interface.h>

bool WIFI_class::nc = true;
bool WIFI_class::scZeroLatch = false;
uint8_t WIFI_class::operatingChannel = 1;
volatile uint8_t WIFI_class::assignedChannel = 0;
uint8_t WIFI_class::assignmentSourceMac[6] = {0};
uint8_t WIFI_class::assignmentSourceChannel = 0;

void WIFI_class::ApplyAssignedChannel() {
    if (assignedChannel == 0) return;
    uint8_t channel = assignedChannel;
    EEPROM.write(0, channel);
    if (!EEPROM.commit()) return;
    const char ack[] = "ACK:SC";
    esp_now_send(assignmentSourceMac, (uint8_t *)ack, sizeof(ack) - 1);
    delay(30);
    operatingChannel = channel;
    assignedChannel = 0;
    wifi_set_channel(operatingChannel);
    nc = false;
}

void WIFI_class::Initialize() {
    EEPROM.begin(16);
    uint8_t savedChannel = EEPROM.read(0);
    operatingChannel = (savedChannel == 1 || savedChannel == 6) ? savedChannel : 1;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    wifi_set_channel(operatingChannel);
    esp_now_init();
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(OnDataRecv);
    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);

    const uint8_t advertiseChannels[] = {1, 6, 11};
    uint8_t advertiseIndex = 0;
    unsigned long lastDisplayTest = millis();
    while (nc) {
        wifi_set_channel(advertiseChannels[advertiseIndex++ % 3]);
        const char hello[] = "HELLO:SC";
        esp_now_send(broadcastAddress, (uint8_t *)hello, sizeof(hello) - 1);
        delay(100);
        ApplyAssignedChannel();

        if (nc && millis() - lastDisplayTest >= 1000) {
            Segment_1 = Segment_1 == 99 ? 0 : Segment_1 + 11;
            Segment_2 = Segment_1;
            Segment_3 = Segment_1;
            lastDisplayTest = millis();
        }
    }
    wifi_set_channel(operatingChannel);
}

void WIFI_class::OnDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
    if (len == 0) return;

    char message[32];
    if (len >= sizeof(message)) return;
    memcpy(message, data, len);
    message[len] = '\0';

    if (strcmp(message, "SET:G1") == 0 || strcmp(message, "SET:G2") == 0) {
        uint8_t channel = strcmp(message, "SET:G1") == 0 ? 1 : 6;
        assignmentSourceChannel = wifi_get_channel();
        memcpy(assignmentSourceMac, mac, 6);
        if (esp_now_is_peer_exist(mac))
            esp_now_del_peer(mac);
        esp_now_add_peer(mac, ESP_NOW_ROLE_COMBO, assignmentSourceChannel, NULL, 0);
        assignedChannel = channel;
        return;
    }

    if (message[0] != '{') return;

    char jsonStr[len + 1];

    memcpy(jsonStr, data, len);
    jsonStr[len] = '\0';
    JsonDocument doc;
    deserializeJson(doc, jsonStr);

    Power_Flag = doc["PS"];
    Colon_Flag = doc["GD"];

    int newSC = doc["SC"];

    // Trigger timeout ONLY once when SC becomes zero
    if (newSC == 0)
    {
        if (!scZeroLatch)
        {
            Timeout_Flag = doc["TF"];   // allow relay once
            scZeroLatch = true;
        }
    }
    else
    {
        // Reset latch when shotclock is non-zero again
        Timeout_Flag = doc["TF"];
        scZeroLatch = false;
    }

    if (doc["CF"] == LOW)
    {
        Segment_1 = (Colon_Flag == GAME_SECONDS) ? doc["TS"] : doc["TM"];
        Segment_2 = (Colon_Flag == GAME_SECONDS) ? doc["TMS"].as<int>() * 10 : doc["TS"];
        Segment_3 = newSC;
    }
    else
    {
        Segment_1 = doc["CH"];
        Segment_2 = doc["CM"];
        Segment_3 = TWO_DIGIT_DASH;
    }

    nc = false;
}
