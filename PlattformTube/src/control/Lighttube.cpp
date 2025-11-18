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
        Serial.println("Driver not present or segmentCount is zero on creation of segments");
        return {};
    }

    const uint16_t totalPixels = static_cast<uint16_t>(driver->getTotalPixelCount());
    if (totalPixels == 0) {
        return {};
    }

    const uint8_t actualSegments = static_cast<uint8_t>(std::min<uint16_t>(segmentCount, totalPixels));
    Serial.print("Should create ");
    Serial.print(static_cast<int>(segmentCount));
    Serial.print(" segments for ");
    Serial.println(static_cast<int>(totalPixels));
    Serial.print("Creating actually");
    Serial.print(static_cast<int>(actualSegments));
    Serial.print(" segments for ");
    Serial.print(static_cast<int>(totalPixels));
    Serial.println(" total pixels.");
    auto segments = std::make_unique<Segment[]>(actualSegments);

    const uint16_t base = totalPixels / actualSegments;     
    uint16_t rem = totalPixels % actualSegments;
    uint16_t cursor = 0;

    for (uint8_t i = 0; i < actualSegments; ++i)
    {
        const uint16_t count = base + (rem ? 1 : 0);
        if (rem) --rem;

        const uint16_t start = cursor;
        const uint16_t end   = (count == 0) ? cursor : static_cast<uint16_t>(cursor + count - 1);

        const uint16_t clampedEnd = std::min<uint16_t>(end, (totalPixels == 0 ? 0 : totalPixels - 1));

        segments[i] = Segment{ totalPixels, start, clampedEnd };
        cursor = static_cast<uint16_t>(clampedEnd + 1);
    }
    Serial.print("Created ");
    Serial.print(static_cast<int>(actualSegments));
    Serial.println("Segments created.");

    return segments;
}

void LightTube::resume(boolean beginDmxReceiver, boolean beginDmxPlayer)
{
    Serial.println("Resume LightTube");
    if(beginDmxPlayer && dmxPlayer != nullptr){
        Serial.println("[LightTube resume] Starting DMX Player...");
        dmxPlayer->begin();
    }
    if(beginDmxReceiver && dmxReceiver != nullptr){
        Serial.println("[LightTube resume] Starting DMX Receiver...");
        dmxReceiver->begin();
    }
    isPaused = false;
    Serial.println("LightTube resumed");
}

void LightTube::pause(boolean stopDmxReceiver, boolean stopDmxPlayer)
{
    Serial.println("Pausing LightTube");
    if(stopDmxPlayer && dmxPlayer != nullptr){
        Serial.println("[LightTube pause] Stopping DMX Player...");
        dmxPlayer->stop();
    }
    if(stopDmxReceiver && dmxReceiver != nullptr){
        Serial.println("[LightTube pause] Stopping DMX Receiver...");
        dmxReceiver->stop();
    }
    isPaused = true;
    Serial.println("LightTube paused");
}

/**
 * @brief Creates the appropriate DMX player based on the DMX mode.
 * Owns the Segment array and returns a unique_ptr to the DMXPlayer.
 */
std::unique_ptr<DMXPlayer> getDMXPlayer(DmxMode dmxMode, ILEDDriver* driver)
{
    if (!driver){
        Serial.println("Driver not present on creation of DMXPlayer");
        return {};
    }

    uint8_t segmentCount = getSegmentCount(dmxMode);
    auto segments = getSegments(segmentCount, driver);

    // If segmentation failed, bail out
    if (!segments) {
        Serial.println("Failed to create segments for DMX Player");
        return {};
    }
    Serial.print("Creating DMX Player for mode ");
    Serial.println(static_cast<int>(dmxMode));

    switch (dmxMode)
    {
        case DmxMode::DMX_1:
            return std::make_unique<DMX1Player>(std::move(segments), segmentCount, driver);

        case DmxMode::DMX_5:
            return std::make_unique<DMX5Player>(std::move(segments), segmentCount, driver);

        case DmxMode::DMX_30:
            return std::make_unique<DMX30Player>(std::move(segments), segmentCount, driver);

        case DmxMode::DMX_40:
            return std::make_unique<DMX40Player>(std::move(segments), segmentCount, driver);

        case DmxMode::DMX_80:
            return std::make_unique<DMX80Player>(std::move(segments), segmentCount, driver);
    }

    return {};
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
LightTube::LightTube(Ticker* ticker, ConfigManager *config) : ticker(ticker), config(config), isPaused(true)
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
    isPaused = false;
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
 */
void LightTube::loop()
{
    if(isPaused){
        Serial.println("LightTube is paused, skipping loop");
        return;
    }
    if(ticker == nullptr || dmxReceiver == nullptr || dmxPlayer == nullptr)
    {
        Serial.println("LightTube loop skipped: ticker, dmxReceiver or dmxPlayer is null");
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
