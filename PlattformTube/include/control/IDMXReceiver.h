#ifndef IDMXRECEIVER_H
#define IDMXRECEIVER_H

#include <Arduino.h>

class IDMXReceiver {
public:
    virtual void begin() = 0;
    virtual bool readData() = 0;
    virtual const uint8_t* getBuffer() = 0;
    virtual uint8_t getChannel(int channel) = 0;
    virtual ~IDMXReceiver() {}
};

#endif // IDMXRECEIVER_H