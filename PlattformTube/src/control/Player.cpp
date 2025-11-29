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

void DMX1Player::loopWithDMX(uint16_t *buffer, uint16_t bufferSize, uint16_t dmxAddr)
{
    if (dmxAddr >= bufferSize || segmentCount() < 1)
    {
        Serial.println("DMX1Player: Invalid DMX address or no segments");
        return;
    }
    driver->setBrightness(buffer[dmxAddr]);
    const Segment &seg = segmentAt(0);
    for (int i = seg.startIdx; i < seg.endIdx; i++)
    {
        driver->setPixelRGB(i, 255, 255, 255, 0);
    }

    driver->show();
}

void DMX1Player::loopWithoutDMX()
{
    
}

// ========================= DMX4Player =========================

void DMX5Player::begin()
{
    Serial.println("------------- DMX 5 Player -------------");
    Serial.println("Segment count: " + String(segmentCount()));
    Serial.println("Channels per segment: " + String(channelsPerSegment));
    Serial.println("Total LEDs: " + String(driver->getTotalPixelCount()));
    Serial.println("----------------------------------------");

    driver->begin();
}

void DMX5Player::stop()
{
    
}

void DMX5Player::loopWithDMX(uint16_t *buffer, uint16_t bufferSize, uint16_t dmxAddr)
{
    if (segmentCount() != 1 || dmxAddr + 4 >= bufferSize)
    {
        Serial.println("DMX5Player: Invalid DMX address or segment count");
        return;
    }
    const uint16_t adr = static_cast<uint16_t>(dmxAddr);
    const Segment &seg = segmentAt(0);
    for (int i = seg.startIdx; i <= seg.endIdx; i++)
    {
        driver->setPixelRGB(i, buffer[adr + 1], buffer[adr + 2], buffer[adr + 3], buffer[adr + 4]);
    }

    driver->show();
}

void DMX5Player::loopWithoutDMX()
{
    
}

// ========================= DMX32Player =========================

void DMX30Player::begin()
{
    Serial.println("------------- DMX 30 Player -------------");
    Serial.println("Segment count: " + String(segmentCount()));
    Serial.println("Channels per segment: " + String(channelsPerSegment));
    Serial.println("Total LEDs: " + String(driver->getTotalPixelCount()));
    
    Serial.println("------------- DMX 30 Driver -------------");
    Serial.println("Driver begin called");
    driver->begin();
    Serial.println("DMX30Player alive");
    Serial.println("-----------------------------------------");
}

void DMX30Player::stop()
{
    
}

void DMX30Player::loopWithDMX(uint16_t *buffer, uint16_t bufferSize, uint16_t dmxAddr)
{
    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const uint16_t adr = static_cast<uint16_t>(dmxAddr) + segIdx * channelsPerSegment;
        const Segment &seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, buffer[adr + 1], buffer[adr + 2], buffer[adr + 3], buffer[adr + 4]);
        }
    }
    driver->show();
}

void DMX30Player::loopWithoutDMX()
{
    
}

// ========================= DMX40Player =========================

void DMX40Player::begin()
{
    Serial.println("------------- DMX 40 Player -------------");
    Serial.println("Segment count: " + String(segmentCount()));
    Serial.println("Channels per segment: " + String(channelsPerSegment));
    Serial.println("Total LEDs: " + String(driver->getTotalPixelCount()));
    Serial.println("-----------------------------------------");

    driver->begin();
}

void DMX40Player::stop()
{

}

void DMX40Player::loopWithDMX(uint16_t *buffer, uint16_t bufferSize, uint16_t dmxAddr)
{
    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const uint16_t adr = static_cast<uint16_t>(dmxAddr) + segIdx * channelsPerSegment;
        const Segment &seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, buffer[adr + 1], buffer[adr + 2], buffer[adr + 3], buffer[adr + 4]);
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
}

void DMX80Player::stop()
{
    
}

void DMX80Player::loopWithDMX(uint16_t *buffer, uint16_t bufferSize, uint16_t dmxAddr)
{
    for (int segIdx = 0; segIdx < segmentCount(); segIdx++)
    {
        const uint16_t adr = static_cast<uint16_t>(dmxAddr) + segIdx * channelsPerSegment;
        const Segment &seg = segmentAt(segIdx);
        for (int i = seg.startIdx; i <= seg.endIdx; i++)
        {
            driver->setPixelRGB(i, buffer[adr + 1], buffer[adr + 2], buffer[adr + 3], buffer[adr + 4]);
        }
    }
    driver->show();
}

void DMX80Player::loopWithoutDMX()
{
    
}
