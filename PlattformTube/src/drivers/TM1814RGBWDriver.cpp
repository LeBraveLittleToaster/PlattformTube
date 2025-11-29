#include "config/HardwareConfig.h"
#include "drivers/TM1814RGBWDriver.h"
#include <NeoPixelBus.h>
#include <Arduino.h>

TM1814RGBWDriver::TM1814RGBWDriver(int dataPinL, int dataPinR, int numLeds)
    : dataPinL(dataPinL), dataPinR(dataPinR), numLeds(numLeds), stripL(numLeds, dataPinL),stripR(numLeds, dataPinR)
{
}

TM1814RGBWDriver::~TM1814RGBWDriver()
{
    delete &stripL;
    delete &stripR;
}

uint8_t TM1814RGBWDriver::getTotalPixelCount()
{
    return numLeds;
}

void TM1814RGBWDriver::begin()
{
    Serial.println("TM1814 LEDs initialized");
    Serial.print("Number of LEDs: ");
    Serial.println(numLeds);
    Serial.print("LED Pin L:");
    Serial.println(dataPinL);
    Serial.print("LED Pin R:");
    Serial.println(dataPinR);

    Serial.println("Starting TM1814 strip L...");
    stripL.Begin();
    Serial.println("Starting TM1814 strip R...");
    stripR.Begin();
    Serial.println("Clearing TM1814 strip...");
    Serial.println("Setting TM1814 settings...");
    stripL.SetPixelSettings(NeoTm1814Settings(165,165,165,165));
    stripR.SetPixelSettings(NeoTm1814Settings(165,165,165,165));
    Serial.println("Filling TM1814 strip with color...");
    stripL.ClearTo(RgbwColor(100, 100, 100, 100));
    stripR.ClearTo(RgbwColor(100, 100, 100, 100));
    Serial.println("Showing TM1814 strip...");
    stripL.Show();
    stripR.Show();
    Serial.println("TM1814 strip ready.");
}


void TM1814RGBWDriver::setBrightness(uint8_t brightness)
{
    //not available for NeoPixelBus
}

void TM1814RGBWDriver::clear()
{
    for (int i = 0; i < numLeds; ++i)
    {
        stripL.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
        stripR.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
    }
}

void TM1814RGBWDriver::show()
{
    stripL.Show();
    stripR.Show();
}

void TM1814RGBWDriver::setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        stripL.SetPixelColor(idx, RgbwColor(r, g, b, w));
        stripR.SetPixelColor(idx, RgbwColor(r, g, b, w));
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

    stripL.SetPixelColor(idx, corrected);
    stripR.SetPixelColor(idx, corrected);
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

    stripL.SetPixelColor(idx, corrected);
    stripR.SetPixelColor(idx, corrected);
}