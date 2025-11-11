#include "control/DMXMode.h"
#include <memory>

uint8_t getSegmentCount(DmxMode tv)
{
    switch (tv)
    {
    case DmxMode::DMX_1:  return 1;
    case DmxMode::DMX_5:  return 1;
    case DmxMode::DMX_30: return 6;
    case DmxMode::DMX_40: return 8;
    case DmxMode::DMX_80: return 16;
    default:              return 0;
    }
}

/**
 * @brief Extracts a single DMX channel value from the buffer.
 * 
 * @param buffer DMX input buffer.
 * @param bufferSize Size of the buffer (0-based addressing expected).
 * @param dmxAdr DMX start address (0-based).
 */
std::unique_ptr<DMX1> getDMX1FromDMXBuffer(uint16_t* buffer, uint16_t bufferSize, uint16_t dmxAdr)
{
    auto dmx1 = std::make_unique<DMX1>();
    const uint16_t need = 1;
    const uint16_t end  = static_cast<uint16_t>(dmxAdr) + need - 1;

    if (buffer == nullptr || bufferSize == 0 || end >= static_cast<uint16_t>(bufferSize))
        return dmx1;

    dmx1->dimmer = buffer[dmxAdr];
    return dmx1;
}

/**
 * @brief Extracts 5 consecutive DMX channel values (Dimmer + R,G,B,W).
 * 
 * @param buffer DMX input buffer.
 * @param bufferSize Size of the buffer (0-based addressing expected).
 * @param dmxAdr DMX start address (0-based).
 */
std::unique_ptr<DMX5> getDMX5FromDMXBuffer(uint16_t* buffer, uint16_t bufferSize, uint16_t dmxAdr)
{
    auto dmx5 = std::make_unique<DMX5>();
    const uint16_t need = 5;
    const uint16_t end  = static_cast<uint16_t>(dmxAdr) + need - 1;

    if (buffer == nullptr){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - buffer is nullptr");
        return dmx5;
    }
    if(bufferSize < need){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - bufferSize: " + String(bufferSize) + ", need: " + String(need));   
        return dmx5;
    } 
    if(end >= static_cast<uint16_t>(bufferSize)){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - end: " + String(end) + " >= bufferSize: " + String(bufferSize));
        return dmx5;
    }
        

    dmx5->dimmer = buffer[dmxAdr + 0];
    dmx5->r      = buffer[dmxAdr + 1];
    dmx5->g      = buffer[dmxAdr + 2];
    dmx5->b      = buffer[dmxAdr + 3];
    dmx5->w      = buffer[dmxAdr + 4];
    return dmx5;
}

/**
 * @brief Extracts 6 segments of 5-channel DMX data (total 30 channels).
 */
std::unique_ptr<DMX30> getDMX30FromDMXBuffer(uint16_t* buffer, uint16_t bufferSize, uint16_t dmxAdr)
{
    auto dmx30 = std::make_unique<DMX30>();
    constexpr uint16_t SEG_COUNT = 6;
    constexpr uint16_t CH_PER    = 5;
    const uint16_t need          = SEG_COUNT * CH_PER;
    const uint16_t end           = static_cast<uint16_t>(dmxAdr) + need - 1;

    if (buffer == nullptr){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - buffer is nullptr");
        return dmx30;
    }
    if(bufferSize < need){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - bufferSize: " + String(bufferSize) + ", need: " + String(need));   
        return dmx30;
    } 
    if(end >= static_cast<uint16_t>(bufferSize)){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - end: " + String(end) + " >= bufferSize: " + String(bufferSize));
        return dmx30;
    }

    for (uint16_t segIdx = 0; segIdx < SEG_COUNT; ++segIdx)
    {
        const uint16_t adr = static_cast<uint16_t>(dmxAdr) + segIdx * CH_PER;
        dmx30->segments[segIdx]->dimmer = buffer[adr + 0];
        dmx30->segments[segIdx]->r      = buffer[adr + 1];
        dmx30->segments[segIdx]->g      = buffer[adr + 2];
        dmx30->segments[segIdx]->b      = buffer[adr + 3];
        dmx30->segments[segIdx]->w      = buffer[adr + 4];
    }
    return dmx30;
}

/**
 * @brief Extracts 8 segments of 5-channel DMX data (total 40 channels).
 */
std::unique_ptr<DMX40> getDMX40FromDMXBuffer(uint16_t* buffer, uint16_t bufferSize, uint16_t dmxAdr)
{
    auto dmx40 = std::make_unique<DMX40>();
    constexpr uint16_t SEG_COUNT = 8;
    constexpr uint16_t CH_PER    = 5;
    const uint16_t need          = SEG_COUNT * CH_PER;
    const uint16_t end           = static_cast<uint16_t>(dmxAdr) + need - 1;

    if (buffer == nullptr || bufferSize < need || end >= static_cast<uint16_t>(bufferSize))
        Serial.println("DMX40Player::getDMX40FromDMXBuffer - Invalid buffer or size. bufferSize: " + String(bufferSize) + ", need: " + String(need) + ", end: " + String(end));
        return dmx40;

    for (uint16_t segIdx = 0; segIdx < SEG_COUNT; ++segIdx)
    {
        const uint16_t adr = static_cast<uint16_t>(dmxAdr) + segIdx * CH_PER;
        dmx40->segments[segIdx]->dimmer = buffer[adr + 0];
        dmx40->segments[segIdx]->r      = buffer[adr + 1];
        dmx40->segments[segIdx]->g      = buffer[adr + 2];
        dmx40->segments[segIdx]->b      = buffer[adr + 3];
        dmx40->segments[segIdx]->w      = buffer[adr + 4];
    }
    return dmx40;
}

/**
 * @brief Extracts 16 segments of 5-channel DMX data (total 80 channels).
 */
std::unique_ptr<DMX80> getDMX80FromDMXBuffer(uint16_t* buffer, uint16_t bufferSize, uint16_t dmxAdr)
{
    auto dmx80 = std::make_unique<DMX80>();
    constexpr uint16_t SEG_COUNT = 16;
    constexpr uint16_t CH_PER    = 5;
    const uint16_t need          = SEG_COUNT * CH_PER;
    const uint16_t end           = static_cast<uint16_t>(dmxAdr) + need - 1;

    
    if (buffer == nullptr){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - buffer is nullptr");
        return dmx80;
    }
    if(bufferSize < need){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - bufferSize: " + String(bufferSize) + ", need: " + String(need));   
        return dmx80;
    } 
    if(end >= static_cast<uint16_t>(bufferSize)){
        Serial.println("DMX5Player::getDMX5FromDMXBuffer - end: " + String(end) + " >= bufferSize: " + String(bufferSize));
        return dmx80;
    }
        
    

    for (uint16_t segIdx = 0; segIdx < SEG_COUNT; ++segIdx)
    {
        const uint16_t adr = static_cast<uint16_t>(dmxAdr) + segIdx * CH_PER;
        dmx80->segments[segIdx]->dimmer = buffer[adr + 0];
        dmx80->segments[segIdx]->r      = buffer[adr + 1];
        dmx80->segments[segIdx]->g      = buffer[adr + 2];
        dmx80->segments[segIdx]->b      = buffer[adr + 3];
        dmx80->segments[segIdx]->w      = buffer[adr + 4];
    }
    return dmx80;
}
