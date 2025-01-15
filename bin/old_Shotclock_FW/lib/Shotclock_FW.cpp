/*******************************************
	DEVELOPED BY JEZREEL TAN - DEC 2023
	jztan25@gmail.com
	(0917) 443 2532
*******************************************/

void IRAM_ATTR LED_Display()
{
    // Clear
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, HIGH);
    digitalWrite(D8, LOW);
    digitalWrite(D9, LOW);
    digitalWrite(D10, LOW);

    if (CurrentSegment == TENS_SEGMENT || CurrentSegment == ONES_SEGMENT || CurrentSegment == SC_SEGMENT) {
        int TENS = (Colon_Flag == GAME_HIDE) ? DISABLE_DIGIT : 
                (CurrentSegment == TENS_SEGMENT) ? Segment_1 / 10 : 
                (CurrentSegment == ONES_SEGMENT) ? Segment_2 / 10 : Segment_3 / 10;
        int ONES = (Colon_Flag == GAME_HIDE) ? DISABLE_DIGIT : 
                (CurrentSegment == TENS_SEGMENT) ? Segment_1 % 10 : 
                (CurrentSegment == ONES_SEGMENT) ? (Colon_Flag == 3 ? Segment_2 % 10 : 15) : Segment_3 % 10;

        // Set Digits
        digitalWrite(D8, (CurrentSegment == TENS_SEGMENT || CurrentSegment == SC_SEGMENT) ? HIGH : LOW);
        digitalWrite(D9, (CurrentSegment == ONES_SEGMENT || CurrentSegment == SC_SEGMENT) ? HIGH : LOW);
        digitalWrite(D10, LOW);
        digitalWrite(D0, (TENS & 1));
        digitalWrite(D1, (TENS & 2) >> 1);
        digitalWrite(D2, (TENS & 4) >> 2);
        digitalWrite(D3, (TENS & 8) >> 3);
        digitalWrite(D4, (ONES & 1));
        digitalWrite(D5, (ONES & 2) >> 1);
        digitalWrite(D6, (ONES & 4) >> 2);
        digitalWrite(D7, (ONES & 8) >> 3);
    }
    else if (Colon_Flag == GAME_MINUTE && CurrentSegment == UPCOL_SEGMENT)
    {
        digitalWrite(D8, LOW);
        digitalWrite(D9, LOW);
        digitalWrite(D10, HIGH);
    }
    else if (Colon_Flag >= GAME_SECONDS && CurrentSegment == DWNCOL_SEGMENT)
    {
        digitalWrite(D8, HIGH);
        digitalWrite(D9, LOW);
        digitalWrite(D10, HIGH);
    }
    else if (Timeout_Flag == TIMEOUT_ON && CurrentSegment == TOUT_SEGMENT)
    {
        digitalWrite(D8, HIGH);
        digitalWrite(D9, HIGH);
        digitalWrite(D10, HIGH);
    }

    CurrentSegment = CurrentSegment == TOUT_SEGMENT ? TENS_SEGMENT : ++CurrentSegment;
}

void Hardware_Initialize()
{
    noInterrupts();
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
    timer.attach(LED_FREQ, LED_Display);
    interrupts();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Segment_1 = Segment_1 == 99 ? 0 : Segment_1 + 11;
        Segment_2 = Segment_2 == 99 ? 0 : Segment_2 + 11;
        Segment_3 = Segment_3 == 99 ? 0 : Segment_3 + 11;
        delay(CONNECTING_INTERVAL);
    }
    http.begin(client, SERVER);
}

void WiFi_GetUpdates()
{
    // TODO: Add reconnecting algorithm!
    if (WiFi.status() == WL_CONNECTED && http.GET() > 0) {
        JsonDocument doc;
        deserializeJson(doc, http.getString());
        Colon_Flag = doc["GAME_DOTS"];
        Segment_1 = (Colon_Flag == GAME_SECONDS) ? doc["TIME_SECOND"] : doc["TIME_MINUTE"];
        Segment_2 = (Colon_Flag == GAME_SECONDS) ? doc["TIME_MS"].as<int>() * 10 : doc["TIME_SECOND"];
        Segment_3 = doc["SHOTCLOCK"];
    }
    delay(FETCH_INTERVAL);
}
