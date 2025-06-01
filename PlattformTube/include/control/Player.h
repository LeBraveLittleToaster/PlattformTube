#include "control/Segment.h"
#include "DMXMode.h"

class DMXPlayer
{
public:
    DMXPlayer(Segment *segments, uint8_t segmentCount, ILEDDriver *leds, DmxMode DmxMode);
    virtual ~DMXPlayer();
    virtual void loopWithDMX(const DMXData *data) = 0;
    virtual void loopWithoutDMX() = 0;
    DmxMode getPlayerDmxType() { return dmxMode; }

private:
    DmxMode dmxMode;
    Segment *segments;
    uint8_t segmentCount;
    ILEDDriver *leds;
};

class DMX1Player : public DMXPlayer
{
public:
    DMX1Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_1) {}
    ~DMX1Player(){}
    void loopWithDMX(const DMXData *data) override;
    void loopWithoutDMX() override;

private:
    DMX1 data;
};

class DMX4Player : public DMXPlayer
{
public:
    DMX4Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_4) {}
    ~DMX4Player(){}
    void loopWithDMX(const DMXData *data) override;
    void loopWithoutDMX() override;

private:
    DMX4 data;
};

class DMX32Player : public DMXPlayer
{
public:
    DMX32Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_32) {}
    ~DMX32Player(){}
    void loopWithDMX(const DMXData *data) override;
    void loopWithoutDMX() override;

private:
    DMX32 data;
};

class DMX64Player : public DMXPlayer
{
public:
    DMX64Player(Segment *segments, uint8_t segmentCount, ILEDDriver *leds)
        : DMXPlayer(segments, segmentCount, leds, DmxMode::DMX_64) {}
    ~DMX64Player(){}
    void loopWithDMX(const DMXData *data) override;
    void loopWithoutDMX() override;

private:
    DMX64 data;
};