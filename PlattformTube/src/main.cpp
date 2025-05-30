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

Lighttube tube {&dmxDriver, &ledDriver, &ticker, 3};

void setup()
{
  delay(1000);
  Serial.begin(115200);
  tube.setup();
  tube.print();
  Serial.println("Starting Ticker...");
  ticker.start();
}

void loop()
{
  tube.loop();
}
