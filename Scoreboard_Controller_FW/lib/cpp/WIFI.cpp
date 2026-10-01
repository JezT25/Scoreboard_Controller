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

void WIFI_class::BuildPacket(ScoreboardPacket &packet)
{
    packet.scoreHome     = IData.SCORE_HOME;
    packet.foulHome      = IData.FOUL_HOME;
    packet.timeoutHome   = IData.TIMEOUT_HOME;
    packet.scoreAway     = IData.SCORE_AWAY;
    packet.foulAway      = IData.FOUL_AWAY;
    packet.timeoutAway   = IData.TIMEOUT_AWAY;
    packet.shotclock     = IData.SHOTCLOCK;
    packet.timeMinute    = IData.TIME_MINUTE;
    packet.timeSecond    = IData.TIME_SECOND;
    packet.timeMs        = IData.TIME_MS;
    packet.clockHour     = IData.CLOCK_HOUR;
    packet.clockMinute   = IData.CLOCK_MINUTE;
    packet.gamePeriod    = IData.GAME_PERIOD;
    packet.gamePosession = IData.GAME_POSESSION;
    packet.gameDots      = IData.GAME_DOTS;
    packet.flags = (IData.TIMEOUT_FLAG  ? PACKET_FLAG_TIMEOUT : 0) |
                   (IData.CLOCK_FLAG    ? PACKET_FLAG_CLOCK   : 0) |
                   (ISystem.POWER_STATE ? PACKET_FLAG_POWER   : 0);
}

void WIFI_class::SendUpdate()
{
    if (ISystem.POWER_STATE != POWER_ON)
        return;

    if (millis() - lastWiFiUpdate >= WIFI_INTERVAL)
    {
        lastWiFiUpdate = millis();

        ScoreboardPacket packet;
        BuildPacket(packet);

        // Frame: [SOF][packed struct][CRC8] so the receiver can resync on corruption
        uint8_t frame[1 + sizeof(ScoreboardPacket) + 1];
        frame[0] = PACKET_SOF;
        memcpy(&frame[1], &packet, sizeof(packet));
        frame[sizeof(frame) - 1] = Protocol_CRC8((uint8_t *)&packet, sizeof(packet));

        Serial3.write(frame, sizeof(frame));
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