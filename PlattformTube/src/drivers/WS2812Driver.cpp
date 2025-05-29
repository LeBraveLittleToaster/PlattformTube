#include "HardwareConfig.h"
#include "WS2812Driver.h"

WS2812Driver::WS2812Driver(int dataPin, int numLeds)
    : dataPin(dataPin), numLeds(numLeds) {
    // Use a fixed-size array to avoid dynamic allocation
    leds = new CRGB[numLeds];
}

void WS2812Driver::begin() {
    FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, numLeds);
    // Serial.println("WS2812 LEDs initialized");
    // Serial.print("Number of LEDs: ");
    // Serial.println(numLeds);
    // Serial.print("LED Pin:");
    // Serial.println(dataPin);
}

void WS2812Driver::setBrightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);
}

void WS2812Driver::clear() {
    for (int i = 0; i < numLeds; ++i) {
        leds[i] = CRGB::Black;
    }
}

void WS2812Driver::show() {
    FastLED.show();
    // Serial.println("Showing WS2812 LEDs");
}

void WS2812Driver::setPixel(int idx, uint8_t r, uint8_t g, uint8_t b) {
    if (idx >= 0 && idx < numLeds) {
        leds[idx] = CRGB(r, g, b);
        // Serial.println("setting pixel ");
        // Serial.print(idx);
        // Serial.print(" to color (");
        // Serial.print(r);
        // Serial.print(", ");
        // Serial.print(g);
        // Serial.print(", ");
        // Serial.print(b);
        // Serial.println(")");
    }
}