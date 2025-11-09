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

// --------------- FREETOS ----------------
TaskHandle_t dmxTaskHandle = nullptr;
TaskHandle_t webTaskHandle = nullptr;

// ------------ Configuration -------------
ConfigManager config{0, 0, DmxReceiverType::ARTNET, DmxMode::DMX_1}; // Only default, values loaded from EEPROM
Ticker ticker{TICKER_INTERVAL_MILLIS};
HttpsAuthServer httpsServer(&config);

LightTube tube{&ticker, &config};

// ----------------- Setup -----------------
void connectToWifi()
{
  Serial.println("Connecting to WiFi");
  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);

  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    if (millis() - t0 > 30000)
    { // 30s hard timeout
      Serial.println("\n[GENERAL] WiFi connect failed. Check credentials or signal.");
      return;
    }
  }
  Serial.print("\n[GENERAL] WiFi OK, IP=");
  Serial.println(WiFi.localIP());
}

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
  {
    httpsServer.loop();
  }
}

// ----------------- Callbacks -----------------
void updateDmxModeCallback(DmxMode dmxMode)
{
  Serial.print("DMX Mode updated to: ");
  Serial.println(static_cast<int>(dmxMode));
  
  tube.pause();
  
  auto newPlayer = getDMXPlayer(dmxMode, &ledDriver);
  if (newPlayer)
  {
    tube.setDmxPlayer(std::move(newPlayer));
    tube.setup();
  }
  else
  {
    Serial.println("Failed to create DMXPlayer for new mode");
  }
}

void updateReceiverCallback(DmxReceiverType dmxReceivers)
{
  Serial.print("DMX Receiver updated to: ");


  tube.pause();
  switch (dmxReceivers)
  {
  case DmxReceiverType::WIRED_DMX:
  {
    Serial.println("WIRED_DMX");
    auto rx = std::make_unique<DMXMAX485>(1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
    tube.setDmxReceiver(std::move(rx));
    break;
  }
  case DmxReceiverType::ARTNET:
  {
    Serial.println("ARTNET");
    auto rx = std::make_unique<Artnet>(&config);
    tube.setDmxReceiver(std::move(rx));
    break;
  }
  default:
    Serial.println("Unknown");
    return;
  }

  tube.setup();
}

// ----------------- Setup/Loop -----------------
void setup()
{

  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial.println("############ WiFi Init ############");
  connectToWifi();
   

  delay(1000);

  if (config.getDmxReceiverType() == DmxReceiverType::WIRED_DMX)
  {
    tube.setDmxReceiver(std::make_unique<DMXMAX485>(1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN));
  }
  else
  {
    tube.setDmxReceiver(std::make_unique<Artnet>(&config));
  }

  delay(1000);

  Serial.println("####### PlattformTube Init ########");

  Serial.println("Setting DMX Player");
  tube.setDmxPlayer(getDMXPlayer(config.getDmxMode(), &ledDriver));

  delay(1000);
  Serial.println("Setup LightTube");
  tube.setup();

  delay(1000);

  Serial.println("####### Register Callback ########");
  Serial.println("Registering Callbacks");
  config.registerDmxModeUpdateCallback(updateDmxModeCallback);
  config.registerReceiverUpdateCallback(updateReceiverCallback);

  Serial.println("########### HTTPS Init ###########");
  httpsServer.begin(WLAN_SSID, WLAN_PASSWORD);

  const UBaseType_t DMX_PRIO = 12;
  const UBaseType_t WEB_PRIO = 4;


  Serial.println("######## DMX Tasks Init ##########");
  xTaskCreatePinnedToCore(DmxTask, "DMX", 4096, nullptr, DMX_PRIO, &dmxTaskHandle, 0); // Core 0
  Serial.println("######## Web Tasks Init ##########");
  xTaskCreatePinnedToCore(WebTask, "WEB", 4096, nullptr, WEB_PRIO, &webTaskHandle, 1); // Core 1
  Serial.println("################################");
  Serial.println("####### Setup Complete #########");
  Serial.println("################################");
}

void loop()
{
  // no-opt
}