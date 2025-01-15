#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define WIFI_SSID      "MyNodeMCU_AP"
#define WIFI_PASSWORD  "password123"

ESP8266WebServer server(80);
String data;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, 1, false);
  IPAddress localIP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(localIP, gateway, subnet);
  server.on("/", []() {
    server.send(200, "text/plain", data);
  });
  server.begin();
}

void loop() {
  server.handleClient();
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    data.trim();
  }
}
