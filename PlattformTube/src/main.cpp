#include <Arduino.h>
#include "Lighttube.h"
#include "HardwareConfig.h"

#if defined(LED_DRIVER_WS2812)
  #include "drivers/WS2812Driver.h"
  WS2812Driver ledDriver(LED_DATA_PIN, NUM_LEDS);
#endif

#if defined(DMX_DRIVER_MAX485)
  #include "drivers/DMXMAX485.h"
  DMXMAX485 dmxDriver(Serial1, DMX_RX_PIN);
#endif

Lighttube tube(&dmxDriver, &ledDriver);

// Simple HSV to RGB conversion for rainbow
CRGB hsv2rgb(uint8_t h, uint8_t s, uint8_t v) {
    CRGB color;
    color.setHSV(h, s, v);
    return color;
}

void testRainbow() {
    unsigned long start = millis();
    while (millis() - start < 5000) {
        for (int i = 0; i < NUM_LEDS; ++i) {
            uint8_t hue = (millis() / 10 + i * (256 / NUM_LEDS)) % 256;
            CRGB color = hsv2rgb(hue, 255, 255);
            ledDriver.setPixel(i, color.r, color.g, color.b);
        }
        ledDriver.show();
        delay(20);
    }
    ledDriver.clear();
    ledDriver.show();
}

void setup() {
  tube.setup();
  testRainbow();
}

void loop() {
  tube.loop();
}