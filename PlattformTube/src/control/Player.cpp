#include "control/Player.h"

DMXPlayer::DMXPlayer(Segment *segments, uint8_t segmentCount, ILEDDriver *driver, DmxMode mode)
    : segments(segments), segmentCount(segmentCount), driver(driver), dmxMode(mode)
{
}

DMXPlayer::~DMXPlayer()
{
    delete driver;
    delete segments;
}

void DMX1Player::begin()
{
    Serial.println("DMX1Player alive");
    driver->begin();
}

void DMX1Player::loopWithDMX(DMXData *data)
{
    Serial.println("Loop DMX1");
    const DMX1 *dmx = static_cast<const DMX1 *>(data);
    if (!dmx)
    {
        Serial.println("DMX1 value invalid!");
        return;
    }

    driver->setBrightness(dmx->dimmer);
    for (int segIdx = 0; segIdx < segmentCount; segIdx++)
    {
        for (int i = segments[segIdx].startIdx; i < segments[segIdx].endIdx; i++)
        {
            driver->setPixelRGB(i, 255, 255, 255, 0);
        }
    }
    driver->show();
}

void DMX1Player::loopWithoutDMX()
{
}

void DMX4Player::begin()
{
    Serial.println("DMX4Player alive");
    driver->begin();
}

void DMX4Player::loopWithDMX(DMXData *data)
{
    Serial.println("Loop DMX4");
    const DMX4 *dmx = static_cast<const DMX4 *>(data);
    if (!dmx)
    {
        Serial.println("DMX4 value invalid!");
        return;
    }
    driver->setBrightness(dmx->dimmer);
    for (int segIdx = 0; segIdx < segmentCount; segIdx++)
    {
        for (int i = segments[segIdx].startIdx; i < segments[segIdx].endIdx; i++)
        {
            driver->setPixelRGB(i, dmx->r, dmx->g, dmx->b, 0);
        }
    }
    driver->show();
}

void DMX4Player::loopWithoutDMX()
{
}

void DMX32Player::begin()
{
    Serial.println("DMX32Player alive");
    driver->begin();
}

void DMX32Player::loopWithDMX(DMXData *data)
{
    Serial.println("Loop DMX32");
    const DMX32 *dmx = static_cast<const DMX32 *>(data);
    if (!dmx)
    {
        Serial.println("DMX32 value invalid!");
        return;
    }
}

void DMX32Player::loopWithoutDMX()
{
}

void DMX64Player::begin()
{
    Serial.println("DMX64Player alive");
    driver->begin();
}

void DMX64Player::loopWithDMX(DMXData *data)
{
    Serial.println("Loop DMX64");
    const DMX64 *dmx = static_cast<const DMX64 *>(data);
    if (!dmx)
    {
        Serial.println("DMX64 value invalid!");
        return;
    }
}

void DMX64Player::loopWithoutDMX()
{
}