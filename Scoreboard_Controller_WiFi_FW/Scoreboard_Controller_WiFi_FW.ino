#include <ESP8266WiFi.h>
#include <espnow.h>

#define CHANNEL1 1
#define CHANNEL2 6
#define CHANNEL3 11

int group;

// Group 1
uint8_t broadcast1_1[] = {0xBC, 0xDD, 0xC2, 0x13, 0x30, 0x9F};
uint8_t broadcast1_2[] = {0x2C, 0xF4, 0x32, 0x79, 0x15, 0x0F};
uint8_t broadcast1_3[] = {0x40, 0x91, 0x51, 0x48, 0x27, 0xEA};

// Group 2
uint8_t broadcast2_1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast2_2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast2_3[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Group All
// Broadcast MAC address (sends to all ESP-NOW peers in range)
uint8_t broadcastALL[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setup()
{
	Serial.begin(115200);

	// Set Wi-Fi mode to station and disconnect from any network
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	// Initialize ESP-NOW
	esp_now_init();

	// Register send callback
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

	if (analogRead(A0) < 450)
	{
		esp_now_add_peer(broadcast1_1, ESP_NOW_ROLE_SLAVE, CHANNEL1, NULL, 0);
		esp_now_add_peer(broadcast1_2, ESP_NOW_ROLE_SLAVE, CHANNEL1, NULL, 0);
		esp_now_add_peer(broadcast1_3, ESP_NOW_ROLE_SLAVE, CHANNEL1, NULL, 0);
		group = 0;
	}
	else if (analogRead(A0) < 900)
	{
		esp_now_add_peer(broadcast2_1, ESP_NOW_ROLE_SLAVE, CHANNEL2, NULL, 0);
		esp_now_add_peer(broadcast2_2, ESP_NOW_ROLE_SLAVE, CHANNEL2, NULL, 0);
		esp_now_add_peer(broadcast2_3, ESP_NOW_ROLE_SLAVE, CHANNEL2, NULL, 0);
		group = 1;
	}
	else
	{
		esp_now_add_peer(broadcastALL, ESP_NOW_ROLE_SLAVE, CHANNEL3, NULL, 0);
		group = 2;
	}
}

void loop()
{
	if (Serial.available())
	{
		char data[250];
		size_t len = Serial.readBytesUntil('\n', data, sizeof(data) - 1);
		data[len] = '\0';

		if (group == 0)
		{
			esp_now_send(broadcast1_1, (uint8_t *)data, len);
			esp_now_send(broadcast1_2, (uint8_t *)data, len);
			esp_now_send(broadcast1_3, (uint8_t *)data, len);
		}
		else if (group == 1)
		{
			esp_now_send(broadcast2_1, (uint8_t *)data, len);
			esp_now_send(broadcast2_2, (uint8_t *)data, len);
			esp_now_send(broadcast2_3, (uint8_t *)data, len);
		}
		else if (group == 2)
		{
			esp_now_send(broadcastALL, (uint8_t *)data, len);
		}
	}
}