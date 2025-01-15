#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>

Ticker timer;

HTTPClient http;
WiFiClient client;
const char* ssid = "MyNodeMCU_AP";
const char* password = "password123";
const char* serverName = "http://192.168.4.1:80/";

volatile int SEG = 1;
volatile int P1 = 0;
volatile int P2 = 0;
volatile int P3 = 0;

void IRAM_ATTR display() {
  int tens = (SEG == 1) ? P1 / 10 : (SEG == 2) ? P2 / 10 : P3 / 10;
  int ones = (SEG == 1) ? P1 % 10 : (SEG == 2) ? P2 % 10 : P3 % 10;

  GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4) | (1 << D5) | (1 << D6) | (1 << D7));
  GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, (1 << D8) | (1 << D9) | (1 << D10));
  GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (SEG == 1 || SEG == 3) ? (1 << D8) : 0);
  GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (SEG == 2 || SEG == 3) ? (1 << D9) : 0);

  digitalWrite(D0, (tens & 1));
  digitalWrite(D1, (tens & 2) >> 1);
  digitalWrite(D2, (tens & 4) >> 2);
  digitalWrite(D3, (tens & 8) >> 3);
  digitalWrite(D4, (ones & 1));
  digitalWrite(D5, (ones & 2) >> 1);
  digitalWrite(D6, (ones & 4) >> 2);
  digitalWrite(D7, (ones & 8) >> 3);

  SEG = (SEG % 3) + 1;
}

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);

  timer.attach(0.01, display);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    P1 = P1 == 99 ? 0 : P1 + 11;
    P2 = P2 == 99 ? 0 : P2 + 11;
    P3 = P3 == 99 ? 0 : P3 + 11;
    delay(1000);
  }
  http.begin(client, serverName);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && http.GET() > 0) {
    JsonDocument doc;
    deserializeJson(doc, http.getString());

    P1 = doc["TIME_MINUTE"];
    P2 = doc["TIME_SECOND"];
    P3 = doc["SHOTCLOCK"];

    delay(100);
  }
}