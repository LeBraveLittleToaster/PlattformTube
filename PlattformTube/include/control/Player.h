#include "control/Segment.h"
#include "DMXMode.h"

template <typename DMXDataType>
class DMXPlayer
{
public:

    virtual ~DMXPlayer() {}
    virtual void loopWithDMX(const DMXDataType& data) = 0;
    virtual void loopWithoutDMX() = 0;
    DmxMode getPlayerDmxType() {return dmxMode;}
    private:
        DmxMode dmxMode;
};

class DMX1Player : public DMXPlayer<DMX1>
{
public:
    DMX1Player(DMX1 data);
    ~DMX1Player();
    void loopWithDMX(const DMX1& data) override;
    void loopWithoutDMX() override;

private:
    DMX1 data;
};

class DMX4Player : public DMXPlayer<DMX4>
{
public:
    DMX4Player(DMX4 data);
    ~DMX4Player();
    void loopWithDMX(const DMX4& data) override;
    void loopWithoutDMX() override;

private:
    DMX4 data;
};

class DMX32Player : public DMXPlayer<DMX32>
{
public:
    DMX32Player(DMX32 data);
    ~DMX32Player();
    void loopWithDMX(const DMX32& data) override;
    void loopWithoutDMX() override;

private:
    DMX32 data;
};

class DMX64Player : public DMXPlayer<DMX64>
{
public:
    DMX64Player(DMX64 data);
    ~DMX64Player();
    void loopWithDMX(const DMX64& data) override;
    void loopWithoutDMX() override;

private:
    DMX64 data;
};