#include <Arduino.h>
#include "control/Lighttube.h"
#include "config/HardwareConfig.h"
#include "control/Ticker.h"
#include <Adafruit_NeoPixel.h>


#if defined(LED_DRIVER_WS2812)
#include "drivers/WS2812Driver.h"
WS2812Driver ledDriver(LED_DATA_PIN, NUM_LEDS);
#endif

#if defined(DMX_DRIVER_MAX485)
#include "drivers/DMXMAX485.h"
DMXMAX485 dmxDriver(Serial1, DMX_RX_PIN);
#endif

Ticker ticker;
Lighttube tube {&dmxDriver, &ledDriver};

void setup()
{
  Serial.begin(115200);
  tube.setup();

  ticker.start();
}

void loop()
{
  tube.loop();
  // One tick in 44hz DMX update frequency
  unsigned long delayMillis = ticker.delayTillNextTick();
  // delayMillis is the amount of time it was delayed (if 0, code is slower then 1/44 of a second and need optimization)
  Serial.println(delayMillis);
}
