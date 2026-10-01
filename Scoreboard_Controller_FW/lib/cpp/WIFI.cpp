/*******************************************
    DEVELOPED BY JEZREEL TAN - DEC 2023
    jztan25@gmail.com
    (0917) 443 2532
*******************************************/
#include "../setup.hpp"

void WIFI_class::StartPairing()
{
    Serial3.println("PAIR");
}

const char *IDATA::toJSON()
{
    static char buffer[350]; // Allocate just enough buffer space

    int length = snprintf(
        buffer, sizeof(buffer),
        "{\"SH\":%d,\"FH\":%d,\"TH\":%d,"
        "\"SA\":%d,\"FA\":%d,\"TA\":%d,"
        "\"SC\":%d,\"TM\":%d,\"TS\":%d,"
        "\"TMS\":%d,\"CH\":%d,\"CM\":%d,"
        "\"GP\":%d,\"GS\":%d,\"GD\":%d,"
        "\"TF\":%d,\"CF\":%d,\"PS\":%d}",
        SCORE_HOME,
        FOUL_HOME,
        TIMEOUT_HOME,
        SCORE_AWAY,
        FOUL_AWAY,
        TIMEOUT_AWAY,
        SHOTCLOCK,
        TIME_MINUTE,
        TIME_SECOND,
        TIME_MS,
        CLOCK_HOUR,
        CLOCK_MINUTE,
        GAME_PERIOD,
        GAME_POSESSION,
        GAME_DOTS,
        TIMEOUT_FLAG,
        CLOCK_FLAG,
        ISystem.POWER_STATE);

    if (length < 0 || length >= sizeof(buffer))
    {
        return "{}"; // Fallback in case of error
    }

    return buffer;
}

void WIFI_class::SendUpdate()
{
    if (ISystem.POWER_STATE != POWER_ON)
        return;

    if (millis() - lastWiFiUpdate >= WIFI_INTERVAL)
    {
        lastWiFiUpdate = millis();
        Serial3.println(IData.toJSON());
    }
}

void WIFI_class::ReadPairingStatus()
{
    static char status[24];
    static uint8_t length = 0;
    while (Serial3.available())
    {
        char value = Serial3.read();
        if (value == '\r')
            continue;
        if (value != '\n')
        {
            if (length < sizeof(status) - 1)
                status[length++] = value;
            else
                length = 0;
            continue;
        }

        status[length] = '\0';
        if (strcmp(status, "PAIR:START") == 0)
        {
            LED.SetPairingMode(true);
            Beep(BEEP_LONG, TONE_LOW);
        }
        else if (strcmp(status, "PAIR:DONE") == 0)
        {
            LED.SetPairingMode(false);
            Beep(BEEP_LONG, TONE_HIGH);
        }
        else if (strcmp(status, "PAIR:FAIL") == 0 || strcmp(status, "PAIR:ALL") == 0 || strcmp(status, "PAIR:ERROR") == 0)
        {
            LED.SetPairingMode(false);
            Beep(BEEP_MED, TONE_LOW);
        }
        else if (strncmp(status, "PAIR:FOUND:", 11) == 0)
            Beep(BEEP_SHORT, TONE_HIGH);
        length = 0;
    }
}