#include "control/Lighttube.h"
#include "config/HardwareConfig.h"

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

Lighttube::Lighttube(IDMXReceiver *dmx, ILEDDriver *driver, Ticker *ticker, int segmentCount)
    : dmx(dmx), leds(driver), ticker(ticker), segmentCount(segmentCount)
{
    uint8_t numLeds = driver->getTotalPixelCount();
    segments = new TubeSegment[numLeds];
    for (uint8_t ledIdx = 0; ledIdx < segmentCount; ledIdx++)
    {
        segments[ledIdx] = TubeSegment{
            numLeds, ledIdx, ledIdx + (numLeds / segmentCount), driver};
    }
}

Lighttube::~Lighttube()
{
    delete[] segments;
}

void Lighttube::setup()
{
    dmx->begin();
    leds->begin();
    config.loadFromEEPROM();
    leds->setBrightness(255);
    leds->clear();
    leds->show();
}

void Lighttube::loop()
{
    // One tick in 44hz DMX update frequency
    // delayMillis is the amount of time it was delayed (if 0, code is slower then 1/44 of a second and need optimization)
    unsigned long delayMillis = ticker->delayTillNextTick();
    
    bool dmxPresent = random(300) == 1;
    for(int segmentIdx = 0; segmentIdx < segmentCount; segmentIdx++){
        if(dmxPresent){
            segments[segmentIdx].loopWithDmx(SegmentValue{255,0,0,0}, DimmerCurve::LINEAR);
        }else {
            segments[segmentIdx].loopWithoutDmx(DimmerCurve::LINEAR);
        }
    }
    
}