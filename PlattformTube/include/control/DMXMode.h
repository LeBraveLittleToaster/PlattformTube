#include <Arduino.h>

#pragma once

/**
 * @enum DmxMode
 * @brief Defines DMX operation modes for different types of lighting control.
 *
 * DMX_1: Single channel (dimmer only, white light).
 * DMX_4: Four channels (dimmer + red, green, blue).
 * DMX_32: Eight segments, each with 4 channels (dimmer + RGB).
 * DMX_64: Sixteen segments, each with 4 channels (dimmer + RGB).
 */
enum DmxMode
{
    DMX_1 = 0,  // INDEX=0 | Dimmer Only, White
    DMX_5 = 1,  // INDEX=1 | Dimmer, R, G, B, , White
    DMX_30 = 2, // INDEX=2 | 6 Segments X {Dimmer, R, G, B, White}
    DMX_40 = 3, // INDEX=3 | 8 Segments X {White, R, G, B, White}
    DMX_80 = 4  // INDEX=4 | 16 Segments X {White, R, G, B, White}
};

uint8_t getSegmentCount (DmxMode tv);

/**
 * @struct DMXData
 * @brief Base struct for DMX data types, allowing polymorphic handling.
 *
 * Contains a virtual destructor to enable safe inheritance.
 */
struct DMXData
{
    virtual ~DMXData() = default;
};

/**
 * @struct DMX1
 * @brief DMX data for single channel dimmer (white light).
 */
struct DMX1 : public DMXData
{
    DMX1() : dimmer(0) {}
    explicit DMX1(uint8_t d) : dimmer(d) {}

    uint8_t dimmer;
};

/**
 * @struct DMX5
 * @brief DMX data for 4-channel RGB lighting (dimmer + R, G, B).
 */
struct DMX5 : public DMXData
{
    DMX5() : dimmer(0), r(0), g(0), b(0), w(0) {}
    DMX5(uint8_t d, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
        : dimmer(d), r(red), g(green), b(blue), w(white) {}

    uint8_t dimmer; ///< Master brightness level (0-255)
    uint8_t r;      ///< Red channel intensity (0-255)
    uint8_t g;      ///< Green channel intensity (0-255)
    uint8_t b;      ///< Blue channel intensity (0-255)
    uint8_t w;     ///< White channel intensity (0-255)
};

/**
 * @struct DMX32
 * @brief DMX data for 8 segments, each with 4 channels (dimmer + RGB).
 *
 * Represents a strip or fixture split into 8 controllable sections.
 */
struct DMX30 : public DMXData
{
    DMX30() = default;

    /**
     * @brief Construct from an array of 8 DMX5 segments.
     * @param segs Array of 8 DMX5 segments.
     */
    DMX30(const DMX5 (&segs)[6])
    {
        for (int i = 0; i < 6; ++i)
            segments[i] = segs[i];
    }

    DMX5 segments[6]; ///< Array of 6 RGB segments
};

/**
 * @struct DMX64
 * @brief DMX data for 16 segments, each with 4 channels (dimmer + RGB).
 *
 * Represents a strip or fixture split into 16 controllable sections.
 */
struct DMX40 : public DMXData
{
    DMX40() = default;

    /**
     * @brief Construct from an array of 8 DMX5 segments.
     * @param segs Array of 8 DMX5 segments.
     */
    DMX40(const DMX5 (&segs)[8])
    {
        for (int i = 0; i < 8; ++i)
            segments[i] = segs[i];
    }

    DMX5 segments[8]; ///< Array of 8 RGB segments
};

/**
 * @struct DMX64
 * @brief DMX data for 16 segments, each with 4 channels (dimmer + RGB).
 *
 * Represents a strip or fixture split into 16 controllable sections.
 */
struct DMX80 : public DMXData
{
    DMX80() = default;

    /**
     * @brief Construct from an array of 16 DMX5 segments.
     * @param segs Array of 16 DMX5 segments.
     */
    DMX80(const DMX5 (&segs)[16])
    {
        for (int i = 0; i < 16; ++i)
            segments[i] = segs[i];
    }

    DMX5 segments[16]; ///< Array of 8 RGB segments
};

/**
 * @brief Extracts DMX1 data (single dimmer channel) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX1 struct with extracted dimmer value.
 */
DMX1 getDMX1FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);

/**
 * @brief Extracts DMX5 data (dimmer + RGB) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX5 struct with extracted dimmer and RGB values.
 */
DMX5 getDMX5FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);

/**
 * @brief Extracts DMX32 data (6 segments of DMX5) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX32 struct with extracted segment data.
 */
DMX30 getDMX30FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);

/**
 * @brief Extracts DMX64 data (8 segments of DMX5) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX64 struct with extracted segment data.
 */
DMX40 getDMX40FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);

/**
 * @brief Extracts DMX64 data (16 segments of DMX5) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX64 struct with extracted segment data.
 */
DMX80 getDMX80FromDMXBuffer(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAdr);
