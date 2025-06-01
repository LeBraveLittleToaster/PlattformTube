#include "control/LightTube.h"
#include "config/HardwareConfig.h"
#include "control/DMXMode.h"

Segment *getSegments(uint8_t segmentCount, ILEDDriver *driver)
{
    uint8_t numLeds = driver->getTotalPixelCount();
    Segment *segments = new Segment[numLeds];

    uint8_t ledsPerSegment = numLeds / segmentCount;
    for (uint8_t segmentIdx = 0; segmentIdx < segmentCount; segmentIdx++)
    {
        uint8_t startIndex = segmentIdx * ledsPerSegment;
        uint8_t endIndex = segmentIdx * ledsPerSegment + (ledsPerSegment - 1);
        if (endIndex + (ledsPerSegment) >= numLeds)
        {
            endIndex = numLeds - 1;
        }
        segments[segmentIdx] = Segment{numLeds, startIndex, endIndex};
    }
    return segments;
}

DMXPlayer *getDMXPlayer(DmxMode dmxMode, int segmentCount, ILEDDriver *driver)
{
    Segment *segments = getSegments(segmentCount, driver);
    switch (dmxMode)
    {
    case DmxMode::DMX_1:
        Serial.println("Creating DMX1 Player");
        return new DMX1Player(segments, segmentCount, driver);
    case DmxMode::DMX_4:
        Serial.println("Creating DMX4 Player");
        return new DMX4Player(segments, segmentCount, driver);
    case DmxMode::DMX_32:
        Serial.println("Creating DMX32 Player");
        return new DMX32Player(segments, segmentCount, driver);
    case DmxMode::DMX_64:
        Serial.println("Creating DMX64 Player");
        return new DMX64Player(segments, segmentCount, driver);
    }
    return nullptr;
}

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

LightTube::LightTube(IDMXReceiver *dmx, Ticker *ticker, ConfigManager *config, DMXPlayer *dmxPlayer)
    : dmx(dmx), ticker(ticker), config(config), dmxPlayer(dmxPlayer)
{
}

LightTube::~LightTube()
{
    // TODO proper destructor
    // delete dmxPlayer;
}

void LightTube::setup()
{
    dmxPlayer->begin();
    /*
    dmx->begin();
    leds->begin();
    config.loadFromEEPROM();
    leds->setBrightness(255);
    leds->clear();
    leds->show();
    */
}

uint8_t b = 0;

void LightTube::loop()
{
    // One tick in 44hz DMX update frequency
    // delayMillis is the amount of time it was delayed (if 0, code is slower then 1/44 of a second and need optimization)
    unsigned long delayMillis = ticker->delayTillNextTick();

    b += 5;
    uint8_t buffer[512];
    buffer[0] = b;
    buffer[1] = 255;
    buffer[2] = 0;
    buffer[3] = 0;

    DMX1 dmx1;
    DMX4 dmx4;
    DMX32 dmx32;
    DMX64 dmx64;
    // TODO: Switch case based DMXMode
    switch (dmxPlayer->dmxMode)
    {
    case DmxMode::DMX_1:
        dmx1 = getDMX1FromDMXBuffer(buffer, 0);
        dmxPlayer->loopWithDMX(&dmx1);
        break;
    case DmxMode::DMX_4:
        dmx4 = getDMX4FromDMXBuffer(buffer, 0);
        dmxPlayer->loopWithDMX(&dmx4);
        break;
    case DmxMode::DMX_32:
        dmx32 = getDMX32FromDMXBuffer(buffer, 0);
        dmxPlayer->loopWithDMX(&dmx32);
        break;
    case DmxMode::DMX_64:
        dmx64 = getDMX64FromDMXBuffer(buffer, 0);
        dmxPlayer->loopWithDMX(&dmx64);
        break;
    default:
        Serial.println("DEFAULTING! CAUTION");
    };

    // bool dmxPresent = random(300) == 1;
}