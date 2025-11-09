#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>

#include "control/Segment.h"
#include "DMXMode.h"

/**
 * @class DMXPlayer
 * @brief Abstract base class for DMX players controlling LED segments.
 *
 * Owns the Segment array via unique_ptr<Segment[]>.
 */
class DMXPlayer
{
public:
    /**
     * @brief Construct a DMXPlayer.
     * @param segments  unique_ptr owning an array of Segment.
     * @param segmentCount Number of segments in the array.
     * @param leds Pointer to LED driver interface (non-owning).
     * @param dmxMode DMX mode this player supports.
     */
    DMXPlayer(std::unique_ptr<Segment[]> segments,
              uint8_t segmentCount,
              ILEDDriver* leds,
              DmxMode dmxMode)
        : dmxMode(dmxMode),
          segments_(std::move(segments)),
          segmentCount_(segmentCount),
          driver(leds)
    {}

    virtual ~DMXPlayer() = default;             // polymorph sicher

    // Nicht kopierbar (Ownership eindeutig)
    DMXPlayer(const DMXPlayer&) = delete;
    DMXPlayer& operator=(const DMXPlayer&) = delete;

    // Bewegbar (Ownership-Transfer erlaubt)
    DMXPlayer(DMXPlayer&&) = default;
    DMXPlayer& operator=(DMXPlayer&&) = default;

    /**
     * @brief Main update loop when DMX data is present.
     */
    virtual void loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr) = 0;

    /**
     * @brief Update loop when no DMX data is available.
     */
    virtual void loopWithoutDMX() = 0;

    /**
     * @brief Initialization and shutdown.
     */
    virtual void begin() = 0;
    virtual void stop() = 0;

    DmxMode getPlayerDmxType() const { return dmxMode; }

protected:
    // Bequemer Zugriff für abgeleitete Klassen
    Segment& segmentAt(std::size_t i)             { return segments_[i]; }
    const Segment& segmentAt(std::size_t i) const { return segments_[i]; }
    Segment* segmentsRaw()                        { return segments_.get(); }
    const Segment* segmentsRaw() const            { return segments_.get(); }
    uint8_t segmentCount() const                  { return segmentCount_; }

    DmxMode dmxMode;
    std::unique_ptr<Segment[]> segments_;   ///< OWNS the array of segments
    uint8_t segmentCount_;                  ///< Number of segments
    ILEDDriver* driver;                     ///< Non-owning LED driver interface
};


/**
 * @class DMX1Player
 * @brief DMXPlayer implementation for DMX_1 mode (single dimmer channel).
 */
class DMX1Player : public DMXPlayer
{
public:
    DMX1Player(std::unique_ptr<Segment[]> segments, uint8_t segmentCount, ILEDDriver* leds)
        : DMXPlayer(std::move(segments), segmentCount, leds, DmxMode::DMX_1) {}
    ~DMX1Player() override = default;

    void begin() override;
    void stop() override;
    void loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX1 data;
};


/**
 * @class DMX4Player
 * @brief DMXPlayer implementation for DMX_4 mode (dimmer + RGB).
 */
class DMX5Player : public DMXPlayer
{
public:
    DMX5Player(std::unique_ptr<Segment[]> segments, uint8_t segmentCount, ILEDDriver* leds)
        : DMXPlayer(std::move(segments), segmentCount, leds, DmxMode::DMX_5) {}
    ~DMX5Player() override = default;

    void begin() override;
    void stop() override;
    void loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX5 data;
};


/**
 * @class DMX32Player
 * @brief DMXPlayer implementation for DMX_32 mode (8 segments with dimmer + RGB).
 */
class DMX30Player : public DMXPlayer
{
public:
    DMX30Player(std::unique_ptr<Segment[]> segments, uint8_t segmentCount, ILEDDriver* leds)
        : DMXPlayer(std::move(segments), segmentCount, leds, DmxMode::DMX_30) {}
    ~DMX30Player() override = default;

    void begin() override;
    void stop() override;
    void loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX30 data;
};


/**
 * @class DMX64Player
 * @brief DMXPlayer implementation for DMX_64 mode (16 segments with dimmer + RGB).
 */
class DMX40Player : public DMXPlayer
{
public:
    DMX40Player(std::unique_ptr<Segment[]> segments, uint8_t segmentCount, ILEDDriver* leds)
        : DMXPlayer(std::move(segments), segmentCount, leds, DmxMode::DMX_40) {}
    ~DMX40Player() override = default;

    void begin() override;
    void stop() override;
    void loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX40 data;
};

/**
 * @class DMX64Player
 * @brief DMXPlayer implementation for DMX_64 mode (16 segments with dimmer + RGB).
 */
class DMX80Player : public DMXPlayer
{
public:
    DMX80Player(std::unique_ptr<Segment[]> segments, uint8_t segmentCount, ILEDDriver* leds)
        : DMXPlayer(std::move(segments), segmentCount, leds, DmxMode::DMX_80) {}
    ~DMX80Player() override = default;

    void begin() override;
    void stop() override;
    void loopWithDMX(uint8_t* buffer, uint8_t bufferSize, uint8_t dmxAddr) override;
    void loopWithoutDMX() override;

private:
    DMX80 data;
};