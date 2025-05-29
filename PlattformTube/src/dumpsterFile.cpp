/*
  for (int i = 0; i < LED_COUNT; i++)
  {
    for (int r = 0; r < 255; r++)
    {
      for (int g = 0; g < 255; g++)
      {
        for (int b = 0; b < 255; b++)
        {
          strip.setPixelColor(i, r, g, b, 0);
          delay(250);
          strip.show();
        }
      }
    }
  }
  strip.clear();
  strip.show();
  
  for (int w = 0; w < 255; w++)
  {
    for (int i = 0; i < LED_COUNT; i++)  
    {
      strip.setPixelColor(i, 0,0, 0, w);
    }
    delay(25);
    strip.show();
  }
  delay(10000);
}
/*


#include "FastLED.h"

#define NUM_LEDS  6    // Numero de leds
#define PIN       21      // PIN ARDUINO

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.clear();

}

void loop() {
  FastLED.setBrightness(100);
  int delayMillis = 500;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    delay(delayMillis);
    FastLED.show();
  }

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    delay(delayMillis);
    FastLED.show();
  }

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
    delay(delayMillis);
    FastLED.show();
  }
  FastLED.setBrightness(0);
  FastLED.show();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    FastLED.show();
  }
  for(int i = 0; i < 255; i++) {
    FastLED.setBrightness(i);
    delay(50);
    FastLED.show();
  }
}*/