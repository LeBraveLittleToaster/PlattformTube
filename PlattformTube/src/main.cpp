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

TaskHandle_t dmxTaskHandle = nullptr;
TaskHandle_t webTaskHandle = nullptr;

ConfigManager config{0, 0, DMXReceivers::ARTNET, DmxMode::DMX_32};

Ticker ticker{TICKER_INTERVAL_MILLIS};

DMXMAX485 *dmx = new DMXMAX485(1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
Artnet *artnet = new Artnet(&config);

LightTube tube{artnet, &ticker, &config, getDMXPlayer(config.getDmxMode(), &ledDriver)};

HttpsAuthServer httpsServer(&config);

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

void updateDmxModeCallback(DmxMode dmxMode)
{
  //TODO: Callback to update DMX Player
  Serial.print("DMX Mode updated to: ");
  Serial.println(dmxMode);
}

void updateReceiverCallback(DMXReceivers dmxReceivers)
{
  Serial.print("DMX Receiver updated to: ");
  tube.deleteDmxReceiver();
  switch (dmxReceivers)
  {
  case DMXReceivers::WIRED_DMX:
    Serial.println("Setting to Wired DMX");
    dmx = new DMXMAX485(1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
    tube.setDmxReceiver(dmx);
    break;
  case DMXReceivers::ARTNET:
    Serial.println("Setting to Wired ARTNET");
    artnet = new Artnet(&config);
    tube.setDmxReceiver(artnet);
    break;

  default:
    Serial.println("Unknown DMX Receiver type");
    break;
  }
  tube.setup();
}

void setup()
{
  delay(1000);
  Serial.begin(115200);
  delay(2000);

  tube.setup();

  config.registerDmxModeUpdateCallback(updateDmxModeCallback);
  config.registerReceiverUpdateCallback(updateReceiverCallback);

  httpsServer.begin(WLAN_SSID, WLAN_PASSWORD);

  const UBaseType_t DMX_PRIO = 12;
  const UBaseType_t WEB_PRIO = 4;

  xTaskCreatePinnedToCore(DmxTask, "DMX", 4096, nullptr, DMX_PRIO, &dmxTaskHandle, 0); // Core 0
  xTaskCreatePinnedToCore(WebTask, "WEB", 4096, nullptr, WEB_PRIO, &webTaskHandle, 1); // Core 1
}

void loop()
{
}
