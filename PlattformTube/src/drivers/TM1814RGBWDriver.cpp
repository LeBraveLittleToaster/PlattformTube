#include "config/HardwareConfig.h"
#include "drivers/TM1814RGBWDriver.h"
#include <NeoPixelBus.h>
#include <Arduino.h>

TM1814RGBWDriver::TM1814RGBWDriver(int dataPin, int numLeds)
    : dataPin(dataPin), numLeds(numLeds), strip(numLeds, dataPin)
{
}

TM1814RGBWDriver::~TM1814RGBWDriver()
{
    delete &strip;
}

uint8_t TM1814RGBWDriver::getTotalPixelCount()
{
    return numLeds;
}

void TM1814RGBWDriver::begin()
{
    Serial.println("WS2812 LEDs initialized");
    Serial.print("Number of LEDs: ");
    Serial.println(numLeds);
    Serial.print("LED Pin:");
    Serial.println(dataPin);

    strip.Begin();
    strip.Show();
}


void TM1814RGBWDriver::setBrightness(uint8_t brightness)
{
    //not available for NeoPixelBus
}

void TM1814RGBWDriver::clear()
{
    for (int i = 0; i < numLeds; ++i)
    {
        strip.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
    }
}

void TM1814RGBWDriver::show()
{
    strip.Show();
}

void TM1814RGBWDriver::setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        strip.SetPixelColor(idx, RgbwColor(r, g, b, w));
    }
}

void TM1814RGBWDriver::setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w)
{
    if (idx < 0 || idx >= numLeds) return;

    uint16_t h16 = uint16_t(h) * 257;   // 0..255 -> 0..65535
    float hf = h16 / 65535.0f;
    float sf = s / 255.0f;
    float vf = v / 255.0f;

    HsbColor hsv(hf, sf, vf);
    RgbwColor c(hsv);
    c.W = w;

    RgbwColor corrected = colorGamma.Correct(c);

    strip.SetPixelColor(idx, corrected);
}

void TM1814RGBWDriver::setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w)
{
    if (idx < 0 || idx >= numLeds) return;

    float hf = h / 65535.0f;   // 0.0 – 1.0
    float sf = s / 255.0f;     // 0.0 – 1.0
    float vf = v / 255.0f;     // 0.0 – 1.0

    HsbColor hsv(hf, sf, vf);      // HSV/HSB in NeoPixelBus
    RgbwColor c(hsv);              // convert to RGBW (W will be 0 here)
    c.W = w;                       // add your separate white channel

    RgbwColor corrected = colorGamma.Correct(c);

    strip.SetPixelColor(idx, corrected);
}