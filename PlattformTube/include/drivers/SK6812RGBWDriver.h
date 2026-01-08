#ifndef SK6812RGBWDRIVER_H
#define SK6812RGBWDRIVER_H

#include "ILEDDriver.h"
#include <Adafruit_NeoPixel.h>

class SK6812RGBWDriver : public ILEDDriver {
public:
    SK6812RGBWDriver(int dataPin, int numLeds);
    void begin() override;
    void setBrightness(uint8_t brightness);
    void clear() override;
    void show() override;
    void setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w) override;
    void setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w) override;
    void setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w) override;
    uint8_t getTotalPixelCount() override;
    ~SK6812RGBWDriver();

private:
    int dataPin;
    int numLeds;
    Adafruit_NeoPixel strip;
};

#endif // SK6812RGBWDRIVER_H