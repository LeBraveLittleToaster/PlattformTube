#include "drivers/ARTNET.h"
#include "control/HttpsAuthServer.h"
#include "config/HardwareConfig.h"

#include <Arduino.h>
#include "control/LightTube.h"
#include "config/HardwareConfig.h"
#include "control/Ticker.h"
#include "control/ConfigManager.h"
#include "drivers/DMXMAX485.h"

#if defined(LED_DRIVER_WS2812)
#include "drivers/WS2812Driver.h"
WS2812Driver ledDriver(LED_DATA_PIN, NUM_LEDS);
#endif

Ticker ticker{TICKER_INTERVAL_MILLIS};
ConfigManager config{0, 0, DMXReceivers::ARTNET, DmxMode::DMX_32};

DMXMAX485 dmx(1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
Artnet artnet(&config);

LightTube tube{&artnet, &ticker, &config, getDMXPlayer(config.getDmxMode(), &ledDriver)};

HttpsAuthServer httpsServer(&config);



TaskHandle_t dmxTaskHandle = nullptr;
TaskHandle_t webTaskHandle = nullptr;

// ----------------- Tasks -----------------
void DmxTask(void *arg)
{
  for (;;)
  {
    tube.loop();
  }
}

void WebTask(void *arg)
{
  for (;;)
    httpsServer.loop();
}

void setup()
{
  delay(1000);
  Serial.begin(115200);
  delay(2000);

  tube.setup();

  httpsServer.begin(WLAN_SSID, WLAN_PASSWORD);

  const UBaseType_t DMX_PRIO = 12;
  const UBaseType_t WEB_PRIO = 4;

  xTaskCreatePinnedToCore(DmxTask, "DMX", 4096, nullptr, DMX_PRIO, &dmxTaskHandle, 0); // Core 0
  xTaskCreatePinnedToCore(WebTask, "WEB", 4096, nullptr, WEB_PRIO, &webTaskHandle, 1); // Core 1
}

void loop()
{
}
