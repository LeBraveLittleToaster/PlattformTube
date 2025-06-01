#include <Arduino.h>

#pragma once

enum DmxMode
{
    DMX_1,  // Dimmer Only, White
    DMX_4,  // Dimmer, R, G, B
    DMX_32, // 8 Segments X {Dimmer, R, G, B}
    DMX_64  // 16 Segments X {Dimmer, R, G, B}
};

struct DMXData {
    virtual ~DMXData() = default;
};

struct DMX1: public DMXData
{
    uint8_t dimmer;
};
struct DMX4: public DMXData
{
    uint8_t dimmer;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct DMX32: public DMXData
{
    DMX4 segments[8];
};
struct DMX64: public DMXData
{
    DMX4 segments[16];
};

DMX1 getDMX1FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);
DMX4 getDMX5FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);
DMX32 getDMX32FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);
DMX64 getDMX64FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);