#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define WIFI_SSID1 "MyNodeMCU_AP"
#define WIFI_SSID2 "MyNodeMCU_AP1"
#define WIFI_PASSWORD1 "password123"
#define WIFI_PASSWORD2 "password123"

ESP8266WebServer server(80);
String data;

void setup() {
	Serial.begin(115200);
	IPAddress localIP(192, 168, 4, 1);
	IPAddress gateway(192, 168, 4, 1);
	IPAddress subnet(255, 255, 255, 0);
	WiFi.softAPdisconnect(true);
	delay(500);
	WiFi.softAPConfig(localIP, gateway, subnet);
	if (analogRead(A0) < 900) {
		WiFi.softAP(WIFI_SSID1, WIFI_PASSWORD1, 11, 1, 4, 15);
	} else {
		WiFi.softAP(WIFI_SSID2, WIFI_PASSWORD2, 11, 1, 4, 15);
	}
	server.on("/", []() {
		server.send(200, "text/plain", data);
	});
	server.begin();
}

void loop() {
	server.handleClient();
	if (Serial.available() > 0) {
		data = Serial.readStringUntil('\n');
	}
}
