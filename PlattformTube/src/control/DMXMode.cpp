#include "control/DMXMode.h"

uint8_t getSegmentCount (DmxMode tv)
{
    switch (tv)
    {
    case DmxMode::DMX_1:
        return 1;
    case DmxMode::DMX_5:
        return 1;
    case DmxMode::DMX_30:
        return 6;
    case DmxMode::DMX_40:
        return 8;
    case DmxMode::DMX_80:
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
DMX1 getDMX1FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr)
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
DMX5 getDMX5FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 5)
    {
        return DMX5{};
    }
    return DMX5{buffer[dmxAdr], buffer[dmxAdr + 1], buffer[dmxAdr + 2], buffer[dmxAdr + 3], buffer[dmxAdr + 4]};
}

/**
 * @brief Extracts 6 segments of 5-channel DMX data (total 32 channels) from the buffer.
 * 
 * @param buffer DMX input buffer (512 bytes).
 * @param dmxAdr DMX start address.
 * @return DMX32 Struct with 8 DMX4 segments. Returns default if out of bounds.
 */
DMX30 getDMX30FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 30)
    {
        return DMX30();
    }
    DMX30 dmx30 = DMX30();
    for (uint8_t segIdx = 0; segIdx < 8; segIdx++)
    {
        uint8_t adr = dmxAdr + segIdx * 5;
        dmx30.segments[segIdx] = DMX5{buffer[adr], buffer[adr + 1], buffer[adr + 2], buffer[adr + 3], buffer[dmxAdr + 4]};
    }
    return dmx30;
}

/**
 * @brief Extracts 8 segments of 4-channel DMX data (total 32 channels) from the buffer.
 * 
 * @param buffer DMX input buffer (512 bytes).
 * @param dmxAdr DMX start address.
 * @return DMX32 Struct with 8 DMX4 segments. Returns default if out of bounds.
 */
DMX40 getDMX40FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 40)
    {
        return DMX40();
    }
    DMX40 dmx40 = DMX40();
    for (uint8_t segIdx = 0; segIdx < 8; segIdx++)
    {
        uint8_t adr = dmxAdr + segIdx * 5;
        dmx40.segments[segIdx] = DMX5{buffer[adr], buffer[adr + 1], buffer[adr + 2], buffer[adr + 3], buffer[dmxAdr + 4]};
    }
    return dmx40;
}

/**
 * @brief Extracts 16 segments of 4-channel DMX data (total 64 channels) from the buffer.
 * 
 * @param buffer DMX input buffer (512 bytes).
 * @param dmxAdr DMX start address.
 * @return DMX64 Struct with 16 DMX4 segments. Returns default if out of bounds.
 */
DMX80 getDMX80FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 80)
    {
        return DMX80();
    }
    DMX80 dmx80 = DMX80();
    for (uint8_t segIdx = 0; segIdx < 16; segIdx++)
    {
        uint8_t adr = dmxAdr + segIdx * 5;
        dmx80.segments[segIdx] = DMX5{buffer[adr], buffer[adr + 1], buffer[adr + 2], buffer[adr + 3], buffer[dmxAdr + 4]};
    }
    return dmx80;
}
