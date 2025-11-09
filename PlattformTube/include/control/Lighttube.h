#ifndef LIGHTTUBE_H
#define LIGHTTUBE_H

#include "drivers/IDMXReceiver.h"
#include "drivers/ILEDDriver.h"
#include "control/Ticker.h"
#include "control/Player.h"
#include "control/ConfigManager.h"
#include "Segment.h"



/**
 * @brief Create an owning array of Segment objects.
 *
 * Divides the strip into @p segmentCount segments as evenly as possible.
 * Ownership is returned as std::unique_ptr<Segment[]>; the caller does NOT
 * delete anything manually.
 *
 * @param segmentCount Number of segments to create (will be clamped to total pixels).
 * @param driver Non-owning pointer to the LED driver (used to query pixel count).
 * @return std::unique_ptr<Segment[]> owning the array (nullptr on error).
 */
std::unique_ptr<Segment[]> getSegments(uint8_t segmentCount, ILEDDriver* driver);

/**
 * @brief Create a DMXPlayer instance for the given mode.
 *
 * Builds the segment array internally and transfers its ownership to the player.
 * The returned std::unique_ptr<DMXPlayer> clearly expresses lifetime ownership.
 *
 * @param dmxMode Target DMX mode (DMX_1, DMX_4, DMX_32, DMX_64).
 * @param driver Non-owning pointer to the LED driver.
 * @return std::unique_ptr<DMXPlayer> (nullptr on error).
 */
std::unique_ptr<DMXPlayer> getDMXPlayer(DmxMode dmxMode, ILEDDriver* driver);

/**
 * @class LightTube
 * @brief High-level controller class managing the light tube's DMX reception and LED updates.
 * 
 * Combines DMX receiving, configuration, timing, and DMX player control to operate an LED tube.
 */
class LightTube {
public:
    /**
     * @brief Construct a LightTube controller instance.
     * 
     * @param dmx Pointer to a DMX receiver interface.
     * @param ticker Pointer to a Ticker instance for timing control.
     * @param config Pointer to configuration manager instance.
     * @param dmxPlayer Pointer to a DMXPlayer controlling LED segments.
     */
    LightTube(Ticker* ticker, ConfigManager* config);

    /**
     * @brief Initialize hardware and internal states.
     * 
     * Should be called once during setup phase.
     */
    void setup();

    /**
     * @brief Main loop to be called repeatedly.
     * 
     * Handles DMX data updates, segment control, and timing.
     */
    void loop();

    /**
     * @brief Output current status or debug info.
     * 
     * Typically prints information about the current light tube state.
     */
    void print();

    void pause();

    void resume();

    
    boolean setDmxPlayer(std::unique_ptr<DMXPlayer> player);

    boolean setDmxReceiver(std::unique_ptr<IDMXReceiver> receiver);
    
    boolean deleteDmxPlayer();

    boolean deleteDmxReceiver();

    /**
     * @brief Destructor to clean up resources.
     */
    ~LightTube();

private:
    std::unique_ptr<IDMXReceiver> dmxReceiver;       ///< DMX receiver interface pointer
    ConfigManager* config;   ///< Configuration manager pointer
    std::unique_ptr<Ticker> ticker;          ///< Timing control pointer
    std::unique_ptr<DMXPlayer> dmxPlayer;    ///< DMX player controlling the LED segments
    SemaphoreHandle_t mutex;   ///< Mutex for thread-safe operations
};

#endif // LIGHTTUBE_H
