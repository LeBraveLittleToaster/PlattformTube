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
DMXMAX485 dmxDriver(DMX_NUM_1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
#endif

Ticker ticker{23};

LightTube tube{&dmxDriver , &ticker, nullptr, getDMXPlayer(DmxMode::DMX_32, SEGMENTS, &ledDriver)};

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
