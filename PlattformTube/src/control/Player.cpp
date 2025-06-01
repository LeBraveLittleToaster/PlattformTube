#include "control/Player.h"

DMXPlayer::DMXPlayer(Segment *segments, uint8_t segmentCount, ILEDDriver *leds, DmxMode mode)
    : segments(segments), segmentCount(segmentCount), leds(leds), dmxMode(mode)
{
}

DMXPlayer::~DMXPlayer()
{
    // Clean up if needed
}

void DMX1Player::loopWithDMX(const DMXData *data)
{
    const DMX1 *dmx = static_cast<const DMX1 *>(data);
    if (!dmx)
    {
        Serial.println("DMX1 value invalid!");
        return;
    }
}

void DMX1Player::loopWithoutDMX()
{
}

void DMX4Player::loopWithDMX(const DMXData *data)
{
    const DMX4 *dmx = static_cast<const DMX4 *>(data);
    if (!dmx)
    {
        Serial.println("DMX4 value invalid!");
        return;
    }
}

void DMX4Player::loopWithoutDMX()
{
}

void DMX32Player::loopWithDMX(const DMXData *data)
{
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

void DMX64Player::loopWithDMX(const DMXData *data)
{
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