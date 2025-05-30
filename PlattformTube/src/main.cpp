#include "control/Ticker.h"
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

Ticker ticker;

void setup()
{
  Serial.begin(115200);
  strip.begin();
  strip.show();
  ticker.start();
}

void loop()
{

  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.clear();
    strip.setPixelColor(i, 0, 0, 255, 0);
    strip.show();
    // One tick in 44hz DMX update frequency
    unsigned long delayMillis = ticker.delayTillNextTick();
    // delayMillis is the amount of time it was delayed (if 0, code is slower then 1/44 of a second and need optimization)
    Serial.println(delayMillis);
  }
}
