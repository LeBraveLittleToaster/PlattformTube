#ifndef ILED_DRIVER_H
#define ILED_DRIVER_H

#include <cstdint>

class ILEDDriver {
public:
    virtual void begin() = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
    virtual void clear() = 0;
    virtual void show() = 0;
    virtual void setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w) = 0;
    virtual void setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w) = 0;
    virtual void setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w) = 0;
    virtual ~ILEDDriver() = default;
};

#endif // ILED_DRIVER_H