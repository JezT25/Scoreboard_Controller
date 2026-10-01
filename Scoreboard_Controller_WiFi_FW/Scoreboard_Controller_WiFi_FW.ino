#include <ESP8266WiFi.h>
#include <espnow.h>
#include <EEPROM.h>
#include <user_interface.h>

#define CHANNEL1 1
#define CHANNEL2 6
#define CHANNEL3 11
#define DEVICE_COUNT 3
#define EEPROM_SIZE 64
#define PAIR_WINDOW_MS 15000
#define PAIRING_MAGIC 0x53435052UL

const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t channels[] = {CHANNEL1, CHANNEL2, CHANNEL3};

struct PairingStorage
{
	uint32_t magic;
	uint8_t group1[DEVICE_COUNT][6];
	uint8_t group2[DEVICE_COUNT][6];
	uint8_t checksum;
};

struct DiscoveredDevice
{
	uint8_t mac[6];
	bool occupied;
	bool acknowledged;
};

PairingStorage pairingStorage;
DiscoveredDevice discovered[DEVICE_COUNT];
int group = 0;
uint8_t groupChannel = CHANNEL1;
bool pairing = false;
bool pairingCompletePending = false;
unsigned long pairingStarted = 0;

uint8_t calculateChecksum(const PairingStorage &storage)
{
	uint8_t checksum = 0x5A;
	for (uint8_t i = 0; i < DEVICE_COUNT; i++)
	{
		for (uint8_t j = 0; j < 6; j++)
		{
			checksum ^= storage.group1[i][j];
			checksum = (checksum << 1) | (checksum >> 7);
			checksum ^= storage.group2[i][j];
			checksum = (checksum << 1) | (checksum >> 7);
		}
	}
	return checksum;
}

bool isEmptyMac(const uint8_t *mac)
{
	bool allFF = true;
	bool allZero = true;
	for (uint8_t i = 0; i < 6; i++)
	{
		allFF = allFF && mac[i] == 0xFF;
		allZero = allZero && mac[i] == 0;
	}
	return allFF || allZero;
}

bool sameMac(const uint8_t *left, const uint8_t *right)
{
	return memcmp(left, right, 6) == 0;
}

uint8_t (*selectedGroupMacs())[6]
{
	return group == 0 ? pairingStorage.group1 : pairingStorage.group2;
}

void loadPairingStorage()
{
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.get(0, pairingStorage);
	if (pairingStorage.magic != PAIRING_MAGIC || pairingStorage.checksum != calculateChecksum(pairingStorage))
	{
		memset(&pairingStorage, 0xFF, sizeof(pairingStorage));
		pairingStorage.magic = PAIRING_MAGIC;
		pairingStorage.checksum = calculateChecksum(pairingStorage);
		EEPROM.put(0, pairingStorage);
		EEPROM.commit();
	}
}

bool savePairingStorage()
{
	pairingStorage.magic = PAIRING_MAGIC;
	pairingStorage.checksum = calculateChecksum(pairingStorage);
	EEPROM.put(0, pairingStorage);
	return EEPROM.commit();
}

void addActiveGroupPeers()
{
	if (group > 1)
		return;
	uint8_t (*macs)[6] = selectedGroupMacs();
	for (uint8_t i = 0; i < DEVICE_COUNT; i++)
	{
		if (!isEmptyMac(macs[i]) && !esp_now_is_peer_exist(macs[i]))
			esp_now_add_peer(macs[i], ESP_NOW_ROLE_COMBO, groupChannel, NULL, 0);
	}
}

void beginPairing()
{
	if (group == 2)
	{
		Serial.println("PAIR:ALL");
		return;
	}

	memset(discovered, 0, sizeof(discovered));
	pairing = true;
	pairingCompletePending = false;
	pairingStarted = millis();
	Serial.println("PAIR:START");
}

int findDiscoveredMac(const uint8_t *mac)
{
	for (int i = 0; i < DEVICE_COUNT; i++)
		if (discovered[i].occupied && sameMac(discovered[i].mac, mac))
			return i;
	return -1;
}

int findSlot(const char *message, const uint8_t *mac)
{
	int existing = findDiscoveredMac(mac);
	if (existing >= 0)
		return existing;
	if (strcmp(message, "HELLO:SB") == 0)
		return discovered[0].occupied ? -1 : 0;
	if (strcmp(message, "HELLO:SC") == 0)
	{
		if (!discovered[1].occupied) return 1;
		if (!discovered[2].occupied) return 2;
	}
	return -1;
}

void assignDevice(int slot)
{
	if (discovered[slot].acknowledged)
		return;
	if (!esp_now_is_peer_exist(discovered[slot].mac))
		esp_now_add_peer(discovered[slot].mac, ESP_NOW_ROLE_COMBO, groupChannel, NULL, 0);
	const char *assignment = group == 0 ? "SET:G1" : "SET:G2";
	esp_now_send(discovered[slot].mac, (uint8_t *)assignment, strlen(assignment));
}

void OnDataRecv(uint8_t *mac, uint8_t *data, uint8_t len)
{
	if (!pairing || len == 0 || len >= 32)
		return;

	char message[32];
	memcpy(message, data, len);
	message[len] = '\0';

	if (strcmp(message, "HELLO:SC") == 0 || strcmp(message, "HELLO:SB") == 0)
	{
		int slot = findSlot(message, mac);
		if (slot < 0)
			return;
		if (!discovered[slot].occupied)
		{
			memcpy(discovered[slot].mac, mac, 6);
			discovered[slot].occupied = true;
			int foundCount = 0;
			for (int i = 0; i < DEVICE_COUNT; i++)
				foundCount += discovered[i].occupied;
			Serial.printf("PAIR:FOUND:%d\n", foundCount);
		}
		assignDevice(slot);
		return;
	}

	int slot = findDiscoveredMac(mac);
	if (slot >= 0 && ((slot == 0 && strcmp(message, "ACK:SB") == 0) ||
		(slot > 0 && strcmp(message, "ACK:SC") == 0)))
	{
		discovered[slot].acknowledged = true;
		bool allAcknowledged = true;
		for (int i = 0; i < DEVICE_COUNT; i++)
			allAcknowledged = allAcknowledged && discovered[i].acknowledged;
		if (allAcknowledged)
			pairingCompletePending = true;
	}
}

void saveCompletedPairing()
{
	uint8_t (*macs)[6] = selectedGroupMacs();
	uint8_t previousMacs[DEVICE_COUNT][6];
	memcpy(previousMacs, macs, sizeof(previousMacs));
	for (uint8_t i = 0; i < DEVICE_COUNT; i++)
		memcpy(macs[i], discovered[i].mac, 6);
	if (!savePairingStorage())
	{
		memcpy(macs, previousMacs, sizeof(previousMacs));
		pairingStorage.checksum = calculateChecksum(pairingStorage);
		pairing = false;
		pairingCompletePending = false;
		Serial.println("PAIR:FAIL");
		return;
	}
	addActiveGroupPeers();
	pairing = false;
	pairingCompletePending = false;
	Serial.println("PAIR:DONE");
}

void sendToAllChannels(const char *data, size_t len)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		wifi_set_channel(channels[i]);
		esp_now_send((uint8_t *)broadcastAddress, (uint8_t *)data, len);
		delay(4);
		yield();
	}
	wifi_set_channel(groupChannel);
}

void sendUpdate(const char *data, size_t len)
{
	if (group == 2)
	{
		sendToAllChannels(data, len);
		return;
	}

	uint8_t (*macs)[6] = selectedGroupMacs();
	for (uint8_t i = 0; i < DEVICE_COUNT; i++)
		if (!isEmptyMac(macs[i]))
			esp_now_send(macs[i], (uint8_t *)data, len);
}

void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(50);
	int a0 = analogRead(A0);
	group = a0 < 450 ? 0 : a0 < 900 ? 1 : 2;
	groupChannel = group == 0 ? CHANNEL1 : group == 1 ? CHANNEL2 : CHANNEL3;
	loadPairingStorage();
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	wifi_set_channel(groupChannel);
	if (esp_now_init() != 0)
	{
		Serial.println("PAIR:ERROR");
		return;
	}
	esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
	esp_now_register_recv_cb(OnDataRecv);
	esp_now_add_peer((uint8_t *)broadcastAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
	addActiveGroupPeers();
}

void loop()
{
	if (pairingCompletePending)
		saveCompletedPairing();
	else if (pairing && millis() - pairingStarted >= PAIR_WINDOW_MS)
	{
		pairing = false;
		Serial.println("PAIR:FAIL");
	}

	if (Serial.available())
	{
		char data[250];
		size_t len = Serial.readBytesUntil('\n', data, sizeof(data) - 1);
		data[len] = '\0';
		if (strcmp(data, "PAIR") == 0)
			beginPairing();
		else if (!pairing && len > 0 && data[0] == '{')
			sendUpdate(data, len);
	}
}