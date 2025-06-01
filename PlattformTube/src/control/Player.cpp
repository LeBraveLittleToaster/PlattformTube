#include "control/Player.h"

/**
 * @brief Constructor for DMXPlayer base class.
 * 
 * @param segments Pointer to an array of segments.
 * @param segmentCount Number of segments.
 * @param driver LED driver instance.
 * @param mode DMX mode (DMX_1, DMX_4, etc.).
 */
DMXPlayer::DMXPlayer(Segment *segments, uint8_t segmentCount, ILEDDriver *driver, DmxMode mode)
    : segments(segments), segmentCount(segmentCount), driver(driver), dmxMode(mode)
{
}

/**
 * @brief Destructor for DMXPlayer.
 * 
 * Cleans up LED driver and segment array. Assumes ownership of both.
 */
DMXPlayer::~DMXPlayer()
{
    delete driver;
    delete[] segments;
}

/**
 * @brief Initializes DMX1Player.
 */
void DMX1Player::begin()
{
    Serial.println("DMX1Player alive");
    driver->begin();
}

/**
 * @brief Processes DMX1 data and updates LED output.
 * 
 * Sets brightness and renders all segments in white.
 * 
 * @param data Pointer to DMX1 data.
 */
void DMX1Player::loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr)
{
    Serial.println("Loop DMX1");
    DMX1 dmx = getDMX1FromDMXBuffer(buffer, dmxAddr);

    driver->setBrightness(dmx.dimmer);
    for (int segIdx = 0; segIdx < segmentCount; segIdx++)
    {
        for (int i = segments[segIdx].startIdx; i < segments[segIdx].endIdx; i++)
        {
            driver->setPixelRGB(i, 255, 255, 255, 0);
        }
    }
    driver->show();
}

/**
 * @brief Called when no DMX data is received (DMX1 mode).
 */
void DMX1Player::loopWithoutDMX()
{
    // No-op for now
}

/**
 * @brief Initializes DMX4Player.
 */
void DMX4Player::begin()
{
    Serial.println("DMX4Player alive");
    driver->begin();
}

/**
 * @brief Processes DMX4 data and updates LED output.
 * 
 * Sets brightness and renders all segments using RGB values.
 * 
 * @param data Pointer to DMX4 data.
 */
void DMX4Player::loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr)
{
    Serial.println("Loop DMX4");
    DMX4 dmx = getDMX4FromDMXBuffer(buffer, dmxAddr);

    driver->setBrightness(dmx.dimmer);
    for (int segIdx = 0; segIdx < segmentCount; segIdx++)
    {
        for (int i = segments[segIdx].startIdx; i <= segments[segIdx].endIdx; i++)
        {
            driver->setPixelRGB(i, dmx.r, dmx.g, dmx.b, 0);
        }
    }
    driver->show();
}

/**
 * @brief Called when no DMX data is received (DMX4 mode).
 */
void DMX4Player::loopWithoutDMX()
{
    // No-op for now
}

/**
 * @brief Initializes DMX32Player.
 */
void DMX32Player::begin()
{
    Serial.println("DMX32Player alive");
    driver->begin();
    driver->setBrightness(255);
}

/**
 * @brief Processes DMX32 data.
 * 
 * Currently not implemented.
 * 
 * @param data Pointer to DMX32 data.
 */
void DMX32Player::loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr)
{
    Serial.println("Loop DMX32");
    DMX32 dmx = getDMX32FromDMXBuffer(buffer, dmxAddr);

    for (int segIdx = 0; segIdx < segmentCount; segIdx++)
    {
        DMX4 dmx4 = dmx.segments[segIdx];
        // TODO: Proper scaling for LED dimmers
        uint16_t r = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.r)+ 127) / 255;;
        uint16_t g = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.g)+ 127) / 255;;
        uint16_t b = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.b)+ 127) / 255;;
        for (int i = segments[segIdx].startIdx; i <= segments[segIdx].endIdx; i++)
        {
            driver->setPixelRGB(i, r,g,b, 0);
        }
    }
    driver->show();
}

/**
 * @brief Called when no DMX data is received (DMX32 mode).
 */
void DMX32Player::loopWithoutDMX()
{
    // No-op for now
}

/**
 * @brief Initializes DMX64Player.
 */
void DMX64Player::begin()
{
    Serial.println("DMX64Player alive");
    driver->begin();
    driver->setBrightness(255);
}

/**
 * @brief Processes DMX64 data.
 * 
 * Currently not implemented.
 * 
 * @param data Pointer to DMX64 data.
 */
void DMX64Player::loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr)
{
    Serial.println("Loop DMX64");
    const DMX64 dmx = getDMX64FromDMXBuffer(buffer, dmxAddr);

    // TODO: Implement segment-based rendering
}

/**
 * @brief Called when no DMX data is received (DMX64 mode).
 */
void DMX64Player::loopWithoutDMX()
{
    // No-op for now
}
