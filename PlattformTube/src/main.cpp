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

// FreeRTOS
TaskHandle_t dmxTaskHandle = nullptr;
TaskHandle_t webTaskHandle = nullptr;

// Konfiguration / Zeitbasis
ConfigManager config{0, 0, DmxReceiverType::ARTNET, DmxMode::DMX_32};
Ticker ticker{TICKER_INTERVAL_MILLIS};

// Server (nutzt nur Referenz auf Config)
HttpsAuthServer httpsServer(&config);

// LightTube besitzt Receiver + Player via unique_ptr
// -> vorausgesetzt: LightTube hat passende API:
//    - setDmxReceiver(std::unique_ptr<IDMXReceiver>)
//    - setPlayer(std::unique_ptr<DMXPlayer>)
//    - setup(), loop()
LightTube tube{ &ticker, &config };

// ----------------- Tasks -----------------
void DmxTask(void* arg)
{
  for (;;) {
    tube.loop();
    vTaskDelay(1);
  }
}

void WebTask(void* arg)
{
  for (;;) {
    httpsServer.loop();
    vTaskDelay(1);
  }
}

// ----------------- Callbacks -----------------
void updateDmxModeCallback(DmxMode dmxMode)
{
  Serial.print("DMX Mode updated to: ");
  Serial.println(static_cast<int>(dmxMode));

  auto newPlayer = getDMXPlayer(dmxMode, &ledDriver);
  if (newPlayer) {
    tube.setDmxPlayer(std::move(newPlayer));
    tube.setup();
  } else {
    Serial.println("Failed to create DMXPlayer for new mode");
  }
}

void updateReceiverCallback(DmxReceiverType dmxReceivers)
{
  Serial.print("DMX Receiver updated to: ");

  switch (dmxReceivers) {
    case DmxReceiverType::WIRED_DMX: {
      Serial.println("WIRED_DMX");
      auto rx = std::make_unique<DMXMAX485>(1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
      tube.setDmxReceiver(std::move(rx));
      break;
    }
    case DmxReceiverType::ARTNET: {
      Serial.println("ARTNET");
      auto rx = std::make_unique<Artnet>(&config);
      tube.setDmxReceiver(std::move(rx));
      break;
    }
    default:
      Serial.println("Unknown");
      return;
  }

  tube.setup(); // Receiver gewechselt -> neu initialisieren
}

// ----------------- Setup/Loop -----------------
void setup()
{
  

  delay(1000);
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting PlattformTube");

  delay(1000);
  
  if (config.getDmxReceiverType() == DmxReceiverType::WIRED_DMX) {
    tube.setDmxReceiver(std::make_unique<DMXMAX485>(1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN));
  } else {
    tube.setDmxReceiver(std::make_unique<Artnet>(&config));
  }

  delay(1000);

  // Player zum aktuellen Mode bauen
  tube.setDmxPlayer(getDMXPlayer(config.getDmxMode(), &ledDriver));

  delay(1000);
  Serial.println("Setup LightTube");
  tube.setup();

  Serial.println("Registering Callbacks");
  config.registerDmxModeUpdateCallback(updateDmxModeCallback);
  config.registerReceiverUpdateCallback(updateReceiverCallback);

  Serial.println("Starting HTTPS Server");
  httpsServer.begin(WLAN_SSID, WLAN_PASSWORD);

  const UBaseType_t DMX_PRIO = 12;
  const UBaseType_t WEB_PRIO = 4;

  Serial.println("Creating Tasks");
  xTaskCreatePinnedToCore(DmxTask, "DMX", 4096, nullptr, DMX_PRIO, &dmxTaskHandle, 0); // Core 0
  Serial.println("Web Task Created");
  Serial.println("Creating Web Task");
  xTaskCreatePinnedToCore(WebTask, "WEB", 4096, nullptr, WEB_PRIO, &webTaskHandle, 1); // Core 1
  Serial.println("Setup complete");
}

void loop()
{
  // nichts — alles in Tasks
}