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
    DMX_1,  // Dimmer Only, White
    DMX_4,  // Dimmer, R, G, B
    DMX_32, // 8 Segments X {Dimmer, R, G, B}
    DMX_64  // 16 Segments X {Dimmer, R, G, B}
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

    uint8_t dimmer; ///< Brightness level (0-255)
};

/**
 * @struct DMX4
 * @brief DMX data for 4-channel RGB lighting (dimmer + R, G, B).
 */
struct DMX4 : public DMXData
{
    DMX4() : dimmer(0), r(0), g(0), b(0) {}
    DMX4(uint8_t d, uint8_t red, uint8_t green, uint8_t blue)
        : dimmer(d), r(red), g(green), b(blue) {}

    uint8_t dimmer; ///< Master brightness level (0-255)
    uint8_t r;      ///< Red channel intensity (0-255)
    uint8_t g;      ///< Green channel intensity (0-255)
    uint8_t b;      ///< Blue channel intensity (0-255)
};

/**
 * @struct DMX32
 * @brief DMX data for 8 segments, each with 4 channels (dimmer + RGB).
 *
 * Represents a strip or fixture split into 8 controllable sections.
 */
struct DMX32 : public DMXData
{
    DMX32() = default;

    /**
     * @brief Construct from an array of 8 DMX4 segments.
     * @param segs Array of 8 DMX4 segments.
     */
    DMX32(const DMX4 (&segs)[8])
    {
        for (int i = 0; i < 8; ++i)
            segments[i] = segs[i];
    }

    DMX4 segments[8]; ///< Array of 8 RGB segments
};

/**
 * @struct DMX64
 * @brief DMX data for 16 segments, each with 4 channels (dimmer + RGB).
 *
 * Represents a strip or fixture split into 16 controllable sections.
 */
struct DMX64 : public DMXData
{
    DMX64() = default;

    /**
     * @brief Construct from an array of 16 DMX4 segments.
     * @param segs Array of 16 DMX4 segments.
     */
    DMX64(const DMX4 (&segs)[16])
    {
        for (int i = 0; i < 16; ++i)
            segments[i] = segs[i];
    }

    DMX4 segments[16]; ///< Array of 16 RGB segments
};

/**
 * @brief Extracts DMX1 data (single dimmer channel) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX1 struct with extracted dimmer value.
 */
DMX1 getDMX1FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);

/**
 * @brief Extracts DMX4 data (dimmer + RGB) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX4 struct with extracted dimmer and RGB values.
 */
DMX4 getDMX4FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);

/**
 * @brief Extracts DMX32 data (8 segments of DMX4) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX32 struct with extracted segment data.
 */
DMX32 getDMX32FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);

/**
 * @brief Extracts DMX64 data (16 segments of DMX4) from a raw DMX buffer.
 * @param buffer Raw DMX data buffer (512 bytes).
 * @param dmxAdr Starting DMX address (1-based).
 * @return DMX64 struct with extracted segment data.
 */
DMX64 getDMX64FromDMXBuffer(uint8_t buffer[512], uint8_t dmxAdr);
