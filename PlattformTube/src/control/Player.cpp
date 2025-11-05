#include "control/Player.h"

// ========================= DMX1Player =========================

void DMX1Player::begin()
{
    Serial.println("DMX1Player alive");
    driver->begin();
}

void DMX1Player::stop()
{
    Serial.println("DMX1Player stopping");
}

void DMX1Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    Serial.println("Loop DMX1");
    DMX1 dmx = getDMX1FromDMXBuffer(buffer, bufferSize, dmxAddr);

    driver->setBrightness(dmx.dimmer);
    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const Segment& seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i < seg.endIdx; i++)
        {
            driver->setPixelRGB(i, 255, 255, 255, 0);
        }
    }
    driver->show();
}

void DMX1Player::loopWithoutDMX()
{
    // No-op for now
}


// ========================= DMX4Player =========================

void DMX4Player::begin()
{
    Serial.println("DMX4Player alive");
    driver->begin();
}

void DMX4Player::stop()
{
    // optional cleanup
}

void DMX4Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    DMX4 dmx = getDMX4FromDMXBuffer(buffer, bufferSize, dmxAddr);

    driver->setBrightness(dmx.dimmer);
    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const Segment& seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, dmx.r, dmx.g, dmx.b, 0);
        }
    }
    driver->show();
}

void DMX4Player::loopWithoutDMX()
{
    // No-op for now
}


// ========================= DMX32Player =========================

void DMX32Player::begin()
{
    Serial.println("DMX32Player alive");
    driver->begin();
    driver->setBrightness(255);
}

void DMX32Player::stop()
{
    // optional cleanup
}

void DMX32Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    Serial.println("Loop DMX32");
    DMX32 dmx = getDMX32FromDMXBuffer(buffer, bufferSize, dmxAddr);

    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const DMX4 dmx4 = dmx.segments[segIdx];

        // TODO: Proper scaling for LED dimmers
        const uint8_t r = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.r) + 127) / 255;
        const uint8_t g = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.g) + 127) / 255;
        const uint8_t b = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.b) + 127) / 255;

        const Segment& seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, r, g, b, 0);
        }
    }
    driver->show();
}

void DMX32Player::loopWithoutDMX()
{
    // No-op for now
}


// ========================= DMX64Player =========================

void DMX64Player::begin()
{
    Serial.println("DMX64Player alive");
    driver->begin();
    driver->setBrightness(255);
}

void DMX64Player::stop()
{
    // optional cleanup
}

void DMX64Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    Serial.println("Loop DMX64");
    const DMX64 dmx = getDMX64FromDMXBuffer(buffer, bufferSize, dmxAddr);

    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const DMX4 dmx4 = dmx.segments[segIdx];

        // TODO: Proper scaling for LED dimmers
        const uint8_t r = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.r) + 127) / 255;
        const uint8_t g = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.g) + 127) / 255;
        const uint8_t b = (static_cast<uint16_t>(dmx4.dimmer) * static_cast<uint16_t>(dmx4.b) + 127) / 255;

        const Segment& seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, r, g, b, 0);
        }
    }
    driver->show();
}

void DMX64Player::loopWithoutDMX()
{
    // No-op for now
}
