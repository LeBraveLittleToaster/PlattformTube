#include <Arduino.h>
#include <memory>

#pragma once

enum DmxMode
{
    DMX_1 = 0,
    DMX_5 = 1,
    DMX_30 = 2,
    DMX_40 = 3,
    DMX_80 = 4
};

uint8_t getSegmentCount(DmxMode tv);

struct DMXData
{
    virtual ~DMXData() = default;
};

struct DMX1 : public DMXData
{
    DMX1() : dimmer(0) {}
    explicit DMX1(uint8_t d) : dimmer(d) {}

    uint8_t dimmer;
};

struct DMX5 : public DMXData
{
    DMX5() : dimmer(0), r(0), g(0), b(0), w(0) {}
    DMX5(uint8_t d, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
        : dimmer(d), r(red), g(green), b(blue), w(white) {}

    uint8_t dimmer;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
};

// --- Now the DMX containers use unique_ptr<DMX5> ---

struct DMX30 : public DMXData
{
    static constexpr int SEG_COUNT = 6;

    DMX30()
    {
        for (int i = 0; i < SEG_COUNT; ++i)
            segments[i] = std::make_unique<DMX5>();
    }

    DMX30(const DMX5 (&segs)[SEG_COUNT])
    {
        for (int i = 0; i < SEG_COUNT; ++i)
            segments[i] = std::make_unique<DMX5>(segs[i]);
    }

    std::unique_ptr<DMX5> segments[SEG_COUNT];
};

struct DMX40 : public DMXData
{
    static constexpr int SEG_COUNT = 8;

    DMX40()
    {
        for (int i = 0; i < SEG_COUNT; ++i)
            segments[i] = std::make_unique<DMX5>();
    }

    DMX40(const DMX5 (&segs)[SEG_COUNT])
    {
        for (int i = 0; i < SEG_COUNT; ++i)
            segments[i] = std::make_unique<DMX5>(segs[i]);
    }

    std::unique_ptr<DMX5> segments[SEG_COUNT];
};

struct DMX80 : public DMXData
{
    static constexpr int SEG_COUNT = 16;

    DMX80()
    {
        for (int i = 0; i < SEG_COUNT; ++i)
            segments[i] = std::make_unique<DMX5>();
    }

    DMX80(const DMX5 (&segs)[SEG_COUNT])
    {
        for (int i = 0; i < SEG_COUNT; ++i)
            segments[i] = std::make_unique<DMX5>(segs[i]);
    }

    std::unique_ptr<DMX5> segments[SEG_COUNT];
};

// --- Extraction function declarations ---

std::unique_ptr<DMX1> getDMX1FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);
std::unique_ptr<DMX5> getDMX5FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);
std::unique_ptr<DMX30> getDMX30FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);
std::unique_ptr<DMX40> getDMX40FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);
std::unique_ptr<DMX80> getDMX80FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);
