#ifndef ILED_DRIVER_H
#define ILED_DRIVER_H

#include <cstdint>

class ILEDDriver {
public:
    virtual void begin() = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
    virtual void clear() = 0;
    virtual void show() = 0;
    virtual void setPixel(int idx, uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual ~ILEDDriver() {}
};

#endif // ILED_DRIVER_H