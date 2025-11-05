#include "control/Segment.h"
#include "config/HardwareConfig.h"
#include "control/DMXMode.h"
#include "control/LightTube.h"
#include "drivers/DMXMAX485.h"
#include "drivers/Artnet.h"

/**
 * @brief Creates LED segments based on total pixel count and segmentCount.
 * Divides as evenly as possible and returns an owning unique_ptr to a Segment array.
 */
std::unique_ptr<Segment[]> getSegments(uint8_t segmentCount, ILEDDriver* driver)
{
    if (!driver || segmentCount == 0) {
        return {}; // nullptr => no segments
    }

    // Use a wide type; strips often exceed 255 pixels
    const uint16_t totalPixels = static_cast<uint16_t>(driver->getTotalPixelCount());
    if (totalPixels == 0) {
        return {};
    }

    // Clamp: can't have more segments than pixels
    const uint8_t actualSegments = static_cast<uint8_t>(std::min<uint16_t>(segmentCount, totalPixels));

    auto segments = std::make_unique<Segment[]>(actualSegments);

    const uint16_t base = totalPixels / actualSegments;     // minimum LEDs per segment
    uint16_t rem = totalPixels % actualSegments;            // leftover LEDs to distribute
    uint16_t cursor = 0;

    for (uint8_t i = 0; i < actualSegments; ++i)
    {
        const uint16_t count = base + (rem ? 1 : 0);
        if (rem) --rem;

        const uint16_t start = cursor;
        const uint16_t end   = (count == 0) ? cursor : static_cast<uint16_t>(cursor + count - 1);

        // Defensive clamp (should be unnecessary if math is right)
        const uint16_t clampedEnd = std::min<uint16_t>(end, (totalPixels == 0 ? 0 : totalPixels - 1));

        segments[i] = Segment{ totalPixels, start, clampedEnd };
        cursor = static_cast<uint16_t>(clampedEnd + 1);
    }

    return segments;
}

/**
 * @brief Creates the appropriate DMX player based on the DMX mode.
 * Owns the Segment array and returns a unique_ptr to the DMXPlayer.
 */
std::unique_ptr<DMXPlayer> getDMXPlayer(DmxMode dmxMode, ILEDDriver* driver)
{
    if (!driver) return {};

    uint8_t segmentCount = getSegmentCount(dmxMode);
    auto segments = getSegments(segmentCount, driver);

    // If segmentation failed, bail out
    if (!segments) return {};

    switch (dmxMode)
    {
        case DmxMode::DMX_1:
            Serial.println("Creating DMX1 Player");
            return std::make_unique<DMX1Player>(std::move(segments), segmentCount, driver);

        case DmxMode::DMX_4:
            Serial.println("Creating DMX4 Player");
            return std::make_unique<DMX4Player>(std::move(segments), segmentCount, driver);

        case DmxMode::DMX_32:
            Serial.println("Creating DMX32 Player");
            return std::make_unique<DMX32Player>(std::move(segments), segmentCount, driver);

        case DmxMode::DMX_64:
            Serial.println("Creating DMX64 Player");
            return std::make_unique<DMX64Player>(std::move(segments), segmentCount, driver);
    }

    return {};
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
LightTube::LightTube(Ticker* ticker, ConfigManager *config) : ticker(ticker), config(config)
{
    mutex = xSemaphoreCreateMutex();
}

/**
 * @brief LightTube destructor.
 *
 * Note: Proper cleanup of dynamically allocated members should be added.
 */
LightTube::~LightTube()
{
    // TODO proper destructor
    dmxPlayer = nullptr;
    dmxReceiver = nullptr;
}

/**
 * @brief Initializes the LightTube (DMX player setup only).
 */
void LightTube::setup()
{
    Serial.println("Setup LightTube");
    config->printConfig();
    Serial.println("Starting DMX");
    dmxReceiver->begin();
    Serial.println("Starting DMX Player");
    dmxPlayer->begin();
    Serial.println("Starting Ticker");
    ticker->start();
    Serial.println("LightTube setup complete");
}

boolean LightTube::deleteDmxPlayer()
{
    if(this->dmxPlayer == nullptr)
    {
        return true;
    }
    xSemaphoreTake(mutex, portMAX_DELAY);
    this->dmxPlayer = nullptr;
    xSemaphoreGive(mutex);
    return true;
}

boolean LightTube::deleteDmxReceiver()
{
    Serial.print("Is dmx null? = ");
    Serial.println(this->dmxReceiver == nullptr ? "true" : "false");
    if(this->dmxReceiver != nullptr)
    {
        Serial.println("DMX Receiver is already null");
        return false;
    }
    xSemaphoreTake(mutex, portMAX_DELAY);
    Serial.println("Deleting DMX Receiver");
    this->dmxReceiver = nullptr;
    Serial.println("DMX Receiver deleted");
    xSemaphoreGive(mutex);
    return true;
}
boolean LightTube::setDmxPlayer(std::unique_ptr<DMXPlayer> player)
{
    Serial.println("Setting new DMX Player");
    xSemaphoreTake(mutex, portMAX_DELAY);
    boolean stoppedPlayer = false;
    if (this->dmxPlayer != nullptr)
    {
        Serial.println("Stopping old DMX Player");
        this->dmxPlayer->stop();
        stoppedPlayer = true;
    }
    Serial.println("Assigning new DMX Player");
    this->dmxPlayer = std::move(player);
    xSemaphoreGive(mutex);
    return stoppedPlayer;
}

boolean LightTube::setDmxReceiver(std::unique_ptr<IDMXReceiver> receiver)
{
    xSemaphoreTake(mutex, portMAX_DELAY);
    boolean stoppedReceiver = false;
    if (this->dmxReceiver != nullptr)
    {
        Serial.println("Stopping old DMX Receiver");
        this->dmxReceiver->stop();
        stoppedReceiver = true;
    }
    Serial.println("Assigning new DMX Receiver");
    this->dmxReceiver = std::move(receiver);
    xSemaphoreGive(mutex);
    return stoppedReceiver;
}

/**
 * @brief Main loop function for LightTube.
 *
 * Reads mock DMX buffer data and routes it to the DMXPlayer based on mode.
 * Uses a ticker to control timing.
 */
void LightTube::loop()
{
    if(ticker == nullptr || dmxReceiver == nullptr || dmxPlayer == nullptr)
    {
        return;
    }

    dmxReceiver->loop();

    // Sync with 44 Hz update rate
    if (ticker->isTickReady())
    {
        dmxReceiver->readData();
        // TODO: Varying buffer size
        dmxPlayer->loopWithDMX(dmxReceiver->getBuffer(), dmxReceiver->getBufferSize(), config->getDmxAddress());
    }
    else
    {
        // TODO: need to handle what happens if no dmx signal was send over long time (3-4 ticks)
        dmxPlayer->loopWithoutDMX();
    }
}
