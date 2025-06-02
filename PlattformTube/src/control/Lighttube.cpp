#include "control/LightTube.h"
#include "config/HardwareConfig.h"
#include "control/DMXMode.h"

/**
 * @brief Creates LED segments based on the total number of pixels and segment count.
 *
 * Divides the LED strip into equal-sized segments and returns an array of Segment objects.
 *
 * @param segmentCount Number of segments to create.
 * @param driver LED driver used to retrieve total pixel count.
 * @return Segment* Pointer to a dynamically allocated array of segments.
 */
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

/**
 * @brief Creates the appropriate DMX player based on the DMX mode.
 *
 * @param dmxMode DMX mode to use (1, 4, 32, or 64 channel).
 * @param segmentCount Number of LED segments.
 * @param driver LED driver instance.
 * @return DMXPlayer* Pointer to the created DMX player instance.
 */
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

/**
 * @brief Linear dimmer curve (identity function).
 *
 * @param value Input brightness.
 * @return uint8_t Output brightness (unchanged).
 */
static uint8_t dimmerCurve1(uint8_t value)
{
    return value;
}

/**
 * @brief Quadratic dimmer curve (S-curve).
 *
 * Makes low brightness levels more subtle, exaggerates highs.
 *
 * @param value Input brightness.
 * @return uint8_t Output brightness (curved).
 */
static uint8_t dimmerCurve2(uint8_t value)
{
    return (value * value) / 255;
}

/**
 * @brief Debug print method for LightTube.
 */
void LightTube::print()
{
    Serial.println("LightTube");
}

/**
 * @brief LightTube constructor.
 *
 * @param dmx DMX receiver interface.
 * @param ticker Ticker used for time-based updates.
 * @param config Config manager.
 * @param dmxPlayer DMX player instance.
 */
LightTube::LightTube(IDMXReceiver *dmx, Ticker *ticker, ConfigManager *config, DMXPlayer *dmxPlayer)
    : dmx(dmx), ticker(ticker), config(config), dmxPlayer(dmxPlayer)
{
}

/**
 * @brief LightTube destructor.
 *
 * Note: Proper cleanup of dynamically allocated members should be added.
 */
LightTube::~LightTube()
{
    // TODO proper destructor
    // delete dmxPlayer;
}

/**
 * @brief Initializes the LightTube (DMX player setup only).
 */
void LightTube::setup()
{
    dmx->begin();
    dmxPlayer->begin();
    ticker->start();

    // Placeholder for full init flow
    /*
    leds->begin();
    config.loadFromEEPROM();
    leds->setBrightness(255);
    leds->clear();
    leds->show();
    */
}

/**
 * @brief Main loop function for LightTube.
 *
 * Reads mock DMX buffer data and routes it to the DMXPlayer based on mode.
 * Uses a ticker to control timing.
 */
void LightTube::loop()
{
    // Sync with 44 Hz update rate
    if (ticker->isTickReady())
    {
        

        // TODO: Refactor with polymorphism instead of switch
        switch (dmxPlayer->getPlayerDmxType())
        {
        case DmxMode::DMX_1:
            dmx->readData();
            dmxPlayer->loopWithDMX(dmx->getBuffer(), 0);
            break;
        case DmxMode::DMX_4:
            dmx->readData();
            dmxPlayer->loopWithDMX(dmx->getBuffer(), 0);
            break;
        case DmxMode::DMX_32:
            dmx->readData();
            dmxPlayer->loopWithDMX(dmx->getBuffer(), 0);
            break;
        case DmxMode::DMX_64:
            dmx->readData();
            dmxPlayer->loopWithDMX(dmx->getBuffer(), 0);
            break;
        default:
            Serial.println("DEFAULTING! CAUTION");
        };
    }
}
