#include "config/HardwareConfig.h"
#include "drivers/SK6812RGBWDriver.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

SK6812RGBWDriver::SK6812RGBWDriver(int dataPin, int numLeds)
    : dataPin(dataPin), numLeds(numLeds), strip(numLeds, dataPin, NEO_GRBW + NEO_KHZ800)
{
}

SK6812RGBWDriver::~SK6812RGBWDriver()
{
    delete &strip;
}

uint8_t SK6812RGBWDriver::getTotalPixelCount()
{
    return numLeds;
}

void SK6812RGBWDriver::begin()
{
    Serial.println("SK6812RGBW LEDs initialized");
    Serial.print("Number of LEDs: ");
    Serial.println(numLeds);
    Serial.print("LED Pin:");
    Serial.println(dataPin);

    strip.setBrightness(255);
    strip.clear();
    strip.show();
}

void SK6812RGBWDriver::setBrightness(uint8_t brightness)
{
    strip.setBrightness(brightness);
}

void SK6812RGBWDriver::clear()
{
    for (int i = 0; i < numLeds; ++i)
    {
        strip.setPixelColor(i, 0, 0, 0, 0);
    }
}

void SK6812RGBWDriver::show()
{
    strip.show();
    // Serial.println("Showing WS2812 LEDs");
}

void SK6812RGBWDriver::setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        strip.setPixelColor(idx, r, g, b, w);
    }
}

void SK6812RGBWDriver::setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w)
{
    uint16_t h16 = (uint16_t)h * 257;
    if (idx >= 0 && idx < numLeds)
    {
        uint32_t rgb = strip.gamma32(strip.ColorHSV(h16, s, v));
        strip.setPixelColor(idx, rgb);
    }
}

void SK6812RGBWDriver::setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        uint32_t rgb = strip.gamma32(strip.ColorHSV(h, s, v));
        strip.setPixelColor(idx, rgb);
    }
}