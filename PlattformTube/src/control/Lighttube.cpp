#include "control/LightTube.h"
#include "config/HardwareConfig.h"
#include "control/DMXMode.h"

// Example dimmer curves
static uint8_t dimmerCurve1(uint8_t value)
{
    // Linear dimmer curve
    return value;
}
static uint8_t dimmerCurve2(uint8_t value)
{
    // S-curve (quadratic)
    return (value * value) / 255;
}

void LightTube::print()
{
    Serial.println("Lighttube");
    Serial.print("Segments: ");
    Serial.println(segmentCount);
    for (uint8_t i = 0; i < segmentCount; i++)
    {
        Serial.print("Segment Index: ");
        Serial.println(i);
        segments[i].print();
    }
}

LightTube::LightTube(IDMXReceiver *dmx, ILEDDriver *driver, Ticker *ticker, int segmentCount)
    : dmx(dmx), leds(driver), ticker(ticker), segmentCount(segmentCount)
{
    uint8_t numLeds = driver->getTotalPixelCount();
    segments = new Segment[numLeds];

    uint8_t ledsPerSegment = numLeds / segmentCount;
    for (uint8_t segmentIdx = 0; segmentIdx < segmentCount; segmentIdx++)
    {
        uint8_t startIndex = segmentIdx * ledsPerSegment;
        uint8_t endIndex = segmentIdx * ledsPerSegment + (ledsPerSegment - 1);
        if (endIndex + (ledsPerSegment) >= numLeds)
        {
            endIndex = numLeds - 1;
        }
        segments[segmentIdx] = Segment{numLeds, startIndex, endIndex, driver};
    }
}

LightTube::~LightTube()
{
    delete[] segments;
}

void LightTube::setup()
{
    dmx->begin();
    leds->begin();
    config.loadFromEEPROM();
    leds->setBrightness(255);
    leds->clear();
    leds->show();
}

void loopPlayer(DMXPlayer *player) {
    switch(player->getPlayerDmxType()) {
        case DmxMode::DMX_1:
            DMX1Player* p1 = static_cast<DMX1Player *>(player);
            p1->loopWithDMX();
            break;
    }
}

void LightTube::loop()
{
    // One tick in 44hz DMX update frequency
    // delayMillis is the amount of time it was delayed (if 0, code is slower then 1/44 of a second and need optimization)
    unsigned long delayMillis = ticker->delayTillNextTick();


    bool dmxPresent = random(300) == 1;
    for (int segmentIdx = 0; segmentIdx < segmentCount; segmentIdx++)
    {
        if (dmxPresent)
        {
            // Else if only for testing
            if (segmentIdx == 0)
            {
                segments[segmentIdx].loopWithDmx(SegmentValue{255, 0, 0, 0}, DimmerCurve::LINEAR);
            }
            else if (segmentIdx == 1)
            {
                segments[segmentIdx].loopWithDmx(SegmentValue{0, 255, 0, 0}, DimmerCurve::LINEAR);
            }
            else if (segmentIdx == 2)
            {
                segments[segmentIdx].loopWithDmx(SegmentValue{0, 0, 255, 0}, DimmerCurve::LINEAR);
            }
            else
            {
                segments[segmentIdx].loopWithDmx(SegmentValue{255, 255, 255, 0}, DimmerCurve::LINEAR);
            }
        }
        else
        {
            segments[segmentIdx].loopWithoutDmx(DimmerCurve::LINEAR);
        }
    }
}