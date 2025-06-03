#include "control/Segment.h"
#include "DMXMode.h"

/**
 * @class DMXPlayer
 * @brief Abstract base class for DMX players controlling LED segments.
 *
 * Manages segments and LED driver based on the DMX mode.
 * Defines interface for handling DMX data and LED updates.
 */
class DMXPlayer
{
public:
    /**
     * @brief Construct a DMXPlayer.
     * @param segments Pointer to array of segments to control.
     * @param segmentCount Number of segments.
     * @param leds Pointer to LED driver interface.
     * @param dmxMode DMX mode this player supports.
     */
    DMXPlayer(Segment *segments, uint8_t segmentCount,ILEDDriver *leds, DmxMode dmxMode);

    /**
     * @brief Virtual destructor for safe polymorphic use.
     */
    virtual ~DMXPlayer();

    /**
     * @brief Main update loop when DMX data is present.
     * @param data Pointer to DMXData containing the latest DMX values.
     */
    virtual void loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr) = 0;

    /**
     * @brief Update loop for when no DMX data is available.
     *        Typically runs fallback animations or idle behavior.
     */
    virtual void loopWithoutDMX() = 0;

    /**
     * @brief Initialization routine, called before main loop.
     */
    virtual void begin() = 0;

    /**
     * @brief Get the DMX mode supported by this player.
     * @return DmxMode enum value.
     */
    DmxMode getPlayerDmxType() { return dmxMode; }

protected:
    DmxMode dmxMode;      ///< DMX mode handled by this player.
    Segment *segments;    ///< Pointer to controlled segments.
    uint8_t segmentCount; ///< Number of segments controlled.
    ILEDDriver *driver;   ///< LED driver interface.
};

/**
 * @class DMX1Player
 * @brief DMXPlayer implementation for DMX_1 mode (single dimmer channel).
 */
class DMX1Player : public DMXPlayer
{
public:
    DMX1Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_1) {}
    ~DMX1Player() override {}

    void begin() override;
    void loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX1 data; ///< Storage for current DMX1 data.
};

/**
 * @class DMX4Player
 * @brief DMXPlayer implementation for DMX_4 mode (dimmer + RGB).
 */
class DMX4Player : public DMXPlayer
{
public:
    DMX4Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_4) {}
    ~DMX4Player() override {}

    void begin() override;
    void loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX4 data; ///< Storage for current DMX4 data.
};

/**
 * @class DMX32Player
 * @brief DMXPlayer implementation for DMX_32 mode (8 segments with dimmer + RGB).
 */
class DMX32Player : public DMXPlayer
{
public:
    DMX32Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_32) {}
    ~DMX32Player() override {}

    void begin() override;
    void loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX32 data; ///< Storage for current DMX32 data.
};

/**
 * @class DMX64Player
 * @brief DMXPlayer implementation for DMX_64 mode (16 segments with dimmer + RGB).
 */
class DMX64Player : public DMXPlayer
{
public:
    DMX64Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_64) {}
    ~DMX64Player() override {}

    void begin() override;
    void loopWithDMX(uint8_t buffer[512], uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX64 data; ///< Storage for current DMX64 data.
};
