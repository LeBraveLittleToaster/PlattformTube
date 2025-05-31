#include "control/Segment.h"
#include "DMXMode.h"

class DMXPlayer
{
public:

    virtual ~DMXPlayer() {}
    virtual void loopWithDMX();
    virtual void loopWithoutDMX();
    DmxMode getPlayerDmxType() {return dmxMode;}
    private:
        DmxMode dmxMode;
};

class DMX1Player : public DMXPlayer
{
public:
    DMX1Player(DMX1 data);
    ~DMX1Player();
    void loopWithDMX() override;

    void loopWithoutDMX() override;

private:
    DMX1 data;
};

class DMX4Player : public DMXPlayer
{
public:
    DMX4Player(DMX4 data);
    ~DMX4Player();
    void loopWithDMX() override;
    void loopWithoutDMX() override;

private:
    DMX4 data;
};

class DMX32Player : public DMXPlayer
{
public:
    DMX32Player(DMX32 data);
    ~DMX32Player();
    void loopWithDMX() override;
    void loopWithoutDMX() override;

private:
    DMX32 data;
};

class DMX64Player : public DMXPlayer
{
public:
    DMX64Player(DMX64 data);
    ~DMX64Player();
    void loopWithDMX() override;
    void loopWithoutDMX() override;

private:
    DMX64 data;
};