
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 21

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 8

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, 255,0,0, 0);
  }
  strip.show();
  delay(1000);
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, 0,255,0, 0);
  }
  strip.show();
  delay(1000);
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, 0,0,255, 0);
  }
  strip.show();
  delay(1000);
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, 255,255,255, 0);
  }
  strip.show();
  delay(5000);
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, 0,0,0, 255);
  }
  strip.show();
  delay(5000);
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, 255,255,255, 255);
  }
  strip.show();
  delay(5000);
}
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