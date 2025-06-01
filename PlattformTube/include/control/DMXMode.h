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
    DMX1() : dimmer(0) {}
    DMX1(uint8_t d) : dimmer(d) {}
    uint8_t dimmer;
};
struct DMX4: public DMXData
{
    DMX4() : dimmer(0), r(0), g(0), b(0) {}
    DMX4(uint8_t d, uint8_t red, uint8_t green, uint8_t blue)
        : dimmer(d), r(red), g(green), b(blue) {}
    uint8_t dimmer;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct DMX32: public DMXData
{
    DMX32() = default;
    DMX32(const DMX4 (&segs)[8]) {
        for (int i = 0; i < 8; ++i)
            segments[i] = segs[i];
    }
    DMX4 segments[8];
};
struct DMX64: public DMXData
{
    DMX64() = default;
    DMX64(const DMX4 (&segs)[16]) {
        for (int i = 0; i < 16; ++i)
            segments[i] = segs[i];
    }
    DMX4 segments[16];
};

DMX1 getDMX1FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);
DMX4 getDMX4FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);
DMX32 getDMX32FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);
DMX64 getDMX64FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);