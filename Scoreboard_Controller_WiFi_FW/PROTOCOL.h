// Single shared copy of the binary protocol used by Scoreboard_Controller_FW,
// Scoreboard_Controller_WiFi_FW, Scoreboard_FW and Shotclock_FW. Edit only here.
#ifndef PROTOCOL_h
#define PROTOCOL_h

#include <stdint.h>
#include <stddef.h>

#define PACKET_SOF 0xAA

#define PACKET_FLAG_TIMEOUT 0x01
#define PACKET_FLAG_CLOCK   0x02
#define PACKET_FLAG_POWER   0x04

#pragma pack(push, 1)
struct ScoreboardPacket
{
    uint8_t scoreHome;
    uint8_t foulHome;
    uint8_t timeoutHome;
    uint8_t scoreAway;
    uint8_t foulAway;
    uint8_t timeoutAway;
    uint8_t shotclock;
    uint8_t timeMinute;
    uint8_t timeSecond;
    uint8_t timeMs;
    uint8_t clockHour;
    uint8_t clockMinute;
    uint8_t gamePeriod;
    uint8_t gamePosession;
    uint8_t gameDots;
    uint8_t flags;
};
#pragma pack(pop)

inline uint8_t Protocol_CRC8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    while (len--)
    {
        crc ^= *data++;
        for (uint8_t i = 0; i < 8; i++)
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x07) : (uint8_t)(crc << 1);
    }
    return crc;
}

#endif
