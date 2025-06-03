#include "control/DMXMode.h"

uint8_t getSegmentCount (DmxMode tv)
{
    switch (tv)
    {
    case DmxMode::DMX_1:
        return 1;
    case DmxMode::DMX_4:
        return 1;
    case DmxMode::DMX_32:
        return 8;
    case DmxMode::DMX_64:
        return 16;
    default:
        return 0;
    }
}

/**
 * @brief Extracts a single DMX channel value from the buffer.
 * 
 * @param buffer DMX input buffer (512 bytes).
 * @param dmxAdr DMX start address (0-based).
 * @return DMX1 Struct containing one DMX channel value. Returns 0 if out of bounds.
 */
DMX1 getDMX1FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 1)
    {
        return DMX1{0};
    }
    return DMX1{buffer[dmxAdr]};
}

/**
 * @brief Extracts four consecutive DMX channel values from the buffer.
 * 
 * @param buffer DMX input buffer (512 bytes).
 * @param dmxAdr DMX start address.
 * @return DMX4 Struct with four DMX channel values. Returns default if out of bounds.
 */
DMX4 getDMX4FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 5)
    {
        return DMX4{};
    }
    return DMX4{buffer[dmxAdr], buffer[dmxAdr + 1], buffer[dmxAdr + 2], buffer[dmxAdr + 3]};
}

/**
 * @brief Extracts 8 segments of 4-channel DMX data (total 32 channels) from the buffer.
 * 
 * @param buffer DMX input buffer (512 bytes).
 * @param dmxAdr DMX start address.
 * @return DMX32 Struct with 8 DMX4 segments. Returns default if out of bounds.
 */
DMX32 getDMX32FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 32)
    {
        return DMX32();
    }
    DMX32 dmx32 = DMX32();
    for (uint8_t segIdx = 0; segIdx < 8; segIdx++)
    {
        uint8_t adr = dmxAdr + segIdx * 4;
        dmx32.segments[segIdx] = DMX4{buffer[adr], buffer[adr + 1], buffer[adr + 2], buffer[adr + 3]};
    }
    return dmx32;
}

/**
 * @brief Extracts 16 segments of 4-channel DMX data (total 64 channels) from the buffer.
 * 
 * @param buffer DMX input buffer (512 bytes).
 * @param dmxAdr DMX start address.
 * @return DMX64 Struct with 16 DMX4 segments. Returns default if out of bounds.
 */
DMX64 getDMX64FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 64)
    {
        return DMX64();
    }
    DMX64 dmx64 = DMX64();
    for (uint8_t segIdx = 0; segIdx < 16; segIdx++)
    {
        uint8_t adr = dmxAdr + segIdx * 4;
        dmx64.segments[segIdx] = DMX4{buffer[adr], buffer[adr + 1], buffer[adr + 2], buffer[adr + 3]};
    }
    return dmx64;
}
