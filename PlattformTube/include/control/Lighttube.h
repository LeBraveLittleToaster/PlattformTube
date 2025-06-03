#include "IDMXReceiver.h"
#include "drivers/ILEDDriver.h"
#include "ConfigManager.h"
#include "control/Ticker.h"
#include "control/Player.h"
#include "Segment.h"

#ifndef LIGHTTUBE_H
#define LIGHTTUBE_H

/**
 * @brief Factory function to create an array of Segment pointers.
 * 
 * @param segmentCount Number of segments to create.
 * @param driver Pointer to an LED driver instance.
 * @return Pointer to the first Segment (likely an array or dynamically allocated).
 * 
 * Note: The ownership and memory management of the returned segments is not specified here.
 */
Segment* getSegments(uint8_t segmentCount, ILEDDriver* driver);

/**
 * @brief Factory function to create a DMXPlayer instance based on mode and segments.
 * 
 * @param dmxMode DMX mode (e.g. DMX_1, DMX_4, DMX_32, DMX_64).
 * @param segmentCount Number of segments to control.
 * @param driver Pointer to an LED driver instance.
 * @return Pointer to a new DMXPlayer configured for the given parameters.
 * 
 * Note: Caller is responsible for managing the lifetime of the returned DMXPlayer.
 */
DMXPlayer* getDMXPlayer(DmxMode dmxMode, ILEDDriver *driver);

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
    LightTube(IDMXReceiver* dmx, Ticker* ticker, ConfigManager* config, DMXPlayer* dmxPlayer);

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

    /**
     * @brief Destructor to clean up resources.
     */
    ~LightTube();

private:
    IDMXReceiver* dmx;       ///< DMX receiver interface pointer
    ConfigManager* config;   ///< Configuration manager pointer
    Ticker* ticker;          ///< Timing control pointer
    DMXPlayer* dmxPlayer;    ///< DMX player controlling the LED segments
};

#endif // LIGHTTUBE_H
