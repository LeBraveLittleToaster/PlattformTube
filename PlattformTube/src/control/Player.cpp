#include "control/Player.h"
#include <memory>

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
    std::unique_ptr<DMX1> dmx = getDMX1FromDMXBuffer(buffer, bufferSize, dmxAddr);

    driver->setBrightness(dmx->dimmer);
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

void DMX5Player::begin()
{
    Serial.println("DMX5Player alive");
    driver->begin();
}

void DMX5Player::stop()
{
    // optional cleanup
}

void DMX5Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    Serial.println("DMX5Player::loopWithDMX");
    std::unique_ptr<DMX5> dmx = getDMX5FromDMXBuffer(buffer, bufferSize, dmxAddr);

    driver->setBrightness(dmx->dimmer);
    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const Segment& seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, dmx->r, dmx->g, dmx->b, 0);
        }
    }
    driver->show();
}

void DMX5Player::loopWithoutDMX()
{
    // No-op for now
}


// ========================= DMX32Player =========================

void DMX30Player::begin()
{
    Serial.println("DMX30Player alive");
    driver->begin();
    driver->setBrightness(255);
}

void DMX30Player::stop()
{
    // optional cleanup
}

void DMX30Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    std::unique_ptr<DMX30> dmx = getDMX30FromDMXBuffer(buffer, bufferSize, dmxAddr);
    Serial.println("DMX30Player::loopWithDMX");
    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        auto& dmx5 = *dmx->segments[segIdx];
        Serial.println("DMX5 Segment " + String(segIdx) + ": Dimmer=" + String(dmx5.dimmer) + " R=" + String(dmx5.r) + " G=" + String(dmx5.g) + " B=" + String(dmx5.b));
        // TODO: Proper scaling for LED dimmers
        const uint8_t r = (static_cast<uint16_t>(dmx5.dimmer) * static_cast<uint16_t>(dmx5.r) + 127) / 255;
        const uint8_t g = (static_cast<uint16_t>(dmx5.dimmer) * static_cast<uint16_t>(dmx5.g) + 127) / 255;
        const uint8_t b = (static_cast<uint16_t>(dmx5.dimmer) * static_cast<uint16_t>(dmx5.b) + 127) / 255;

        const Segment& seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            Serial.println("Setting pixel " + String(i) + " to R:" + String(r) + " G:" + String(g) + " B:" + String(b));
            driver->setPixelRGB(i, r, g, b, 0);
        }
    }
    driver->show();
}

void DMX30Player::loopWithoutDMX()
{
    // No-op for now
}


// ========================= DMX40Player =========================

void DMX40Player::begin()
{
    Serial.println("DMX40Player alive");
    driver->begin();
    driver->setBrightness(255);
}

void DMX40Player::stop()
{
    // optional cleanup
}

void DMX40Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    Serial.println("DMX40Player::loopWithDMX");
    std::unique_ptr<DMX40> dmx = getDMX40FromDMXBuffer(buffer, bufferSize, dmxAddr);

    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        
        auto& dmx5 = *dmx->segments[segIdx];

        // TODO: Proper scaling for LED dimmers
        
        const Segment& seg = segmentAt(segIdx);
        // TODO: white is ignored for now
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, dmx5.r, dmx5.g, dmx5.b, 0);
        }
    }
    driver->show();
}

void DMX40Player::loopWithoutDMX()
{
    // No-op for now
}

// ========================= DMX40Player =========================

void DMX80Player::begin()
{
    Serial.println("DMX80Player alive");
    driver->begin();
    driver->setBrightness(255);
}

void DMX80Player::stop()
{
    // optional cleanup
}

void DMX80Player::loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr)
{
    std::unique_ptr<DMX80> dmx = getDMX80FromDMXBuffer(buffer, bufferSize, dmxAddr);

    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        auto& dmx5 = *dmx->segments[segIdx];

        // TODO: Proper scaling for LED dimmers
        const uint8_t r = (static_cast<uint16_t>(dmx5.dimmer) * static_cast<uint16_t>(dmx5.r) + 127) / 255;
        const uint8_t g = (static_cast<uint16_t>(dmx5.dimmer) * static_cast<uint16_t>(dmx5.g) + 127) / 255;
        const uint8_t b = (static_cast<uint16_t>(dmx5.dimmer) * static_cast<uint16_t>(dmx5.b) + 127) / 255;

        const Segment& seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, r, g, b, 0);
        }
    }
    driver->show();
}

void DMX80Player::loopWithoutDMX()
{
    // No-op for now
}

