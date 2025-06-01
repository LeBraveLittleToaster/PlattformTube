 #include <Arduino.h>
 #include "control/LightTube.h"
 #include "config/HardwareConfig.h"
 #include "control/Ticker.h"

#if defined(LED_DRIVER_WS2812)
#include "drivers/WS2812Driver.h"
WS2812Driver ledDriver(LED_DATA_PIN, NUM_LEDS);
#endif

#if defined(DMX_DRIVER_MAX485)
#include "drivers/DMXMAX485.h"
DMXMAX485 dmxDriver(Serial1, DMX_RX_PIN);
#endif

Ticker ticker;

LightTube tube{&dmxDriver , &ticker, nullptr, getDMXPlayer(DmxMode::DMX_4, SEGMENTS, &ledDriver)};

void setup()
{
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  tube.setup();
}



void loop()
{
  tube.loop();
}
