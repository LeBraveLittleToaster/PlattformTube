#include "control/DMXMode.h"

DMX4 createDMX4(uint8_t dimmer, uint8_t r, uint8_t g, uint8_t b)
{
    DMX4 dmx4 = DMX4();
    dmx4.dimmer = dimmer;
    dmx4.r = r;
    dmx4.g = g;
    dmx4.b = b;
    return dmx4;
}

DMX1 getDMX1FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 1)
    {
        return DMX1();
    }
    DMX1 dmx1 = DMX1();
    dmx1.dimmer = buffer[dmxAdr];
    return dmx1;
}

DMX4 getDMX4FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 5)
    {
        return DMX4();
    }
    return createDMX4(buffer[dmxAdr], buffer[dmxAdr + 1],buffer[dmxAdr + 2],buffer[dmxAdr + 3]);
}

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
        dmx32.segments[segIdx] = createDMX4(buffer[adr], buffer[adr + 1],buffer[adr + 2],buffer[adr + 3]);
    }
    return dmx32;
}

DMX64 getDMX64FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr)
{
    if (dmxAdr > 512 - 64 || dmxAdr < 0)
    {
        return DMX64();
    }
    DMX64 dmx64 = DMX64();
    for (uint8_t segIdx = 0; segIdx < 16; segIdx++)
    {
        uint8_t adr = dmxAdr + segIdx * 4;
        dmx64.segments[segIdx] = createDMX4(buffer[adr], buffer[adr + 1],buffer[adr + 2],buffer[adr + 3]);
    }
    return dmx64;
}