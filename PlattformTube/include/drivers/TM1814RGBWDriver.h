#ifndef WS2812DRIVER_H
#define WS2812DRIVER_H

#include "ILEDDriver.h"
#include <Arduino.h>
#include <NeoPixelBus.h>

class TM1814RGBWDriver : public ILEDDriver {
public:
    TM1814RGBWDriver(int dataPinL, int dataPinR, int numLeds);
    void begin() override;
    void setBrightness(uint8_t brightness);
    void clear() override;
    void show() override;
    void setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w) override;
    void setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w) override;
    void setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w) override;
    uint8_t getTotalPixelCount() override;
    ~TM1814RGBWDriver();

private:
    int dataPinL;
    int dataPinR;
    int numLeds;
    NeoPixelBus<NeoWrgbTm1814Feature, NeoTm1814Method> stripL;
    NeoPixelBus<NeoWrgbTm1814Feature, NeoTm1814Method> stripR;
    NeoGamma<NeoGammaTableMethod> colorGamma;
};

#endif // WS2812DRIVER_H