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
    Serial.println("LightTube");
}

DMXPlayer* getDMXPlayer(DmxMode dmxMode, int segmentCount, ILEDDriver *driver)
{
    uint8_t numLeds = driver->getTotalPixelCount();

    Segment* segments = new Segment[numLeds];

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
    switch(dmxMode) {
        case DmxMode::DMX_1:
            return new DMX1Player(segments, segmentCount, driver);
        case DmxMode::DMX_4:
            return new DMX4Player(segments, segmentCount, driver);
        case DmxMode::DMX_32:
            return new DMX32Player(segments, segmentCount, driver);
        case DmxMode::DMX_64:
            return new DMX64Player(segments, segmentCount, driver);
    }
    return nullptr;
}

LightTube::LightTube(IDMXReceiver *dmx, Ticker *ticker, ConfigManager* config, DMXPlayer* dmxPlayer)
: dmx(dmx), ticker(ticker), config(config), dmxPlayer(dmxPlayer) {

}

LightTube::~LightTube()
{
    //TODO proper destructor
    //delete dmxPlayer;
}

void LightTube::setup()
{
    /*
    dmx->begin();
    leds->begin();
    config.loadFromEEPROM();
    leds->setBrightness(255);
    leds->clear();
    leds->show();
    */
}

void LightTube::loop()
{
    // One tick in 44hz DMX update frequency
    // delayMillis is the amount of time it was delayed (if 0, code is slower then 1/44 of a second and need optimization)
    unsigned long delayMillis = ticker->delayTillNextTick();

    bool dmxPresent = random(300) == 1;
}