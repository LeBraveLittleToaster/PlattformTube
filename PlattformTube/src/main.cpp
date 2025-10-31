/*
#include <Arduino.h>
 #include "control/LightTube.h"
 #include "config/HardwareConfig.h"
 #include "control/Ticker.h"
 #include "control/ConfigManager.h"

#if defined(LED_DRIVER_WS2812)
#include "drivers/WS2812Driver.h"
WS2812Driver ledDriver(LED_DATA_PIN, NUM_LEDS);
#endif

#if defined(DMX_DRIVER_MAX485)
#include "drivers/DMXMAX485.h"
DMXMAX485 dmxDriver(DMX_NUM_1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
#endif

Ticker ticker{TICKER_INTERVAL_MILLIS};
ConfigManager conf;

LightTube tube{&dmxDriver , &ticker, &conf, getDMXPlayer(DmxMode::DMX_32, &ledDriver)};

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
*/

/*

###################### ARTNET EXAMPLE ########################

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>
#include <config/HardwareConfig.h>


const int numberOfChannels = NUM_LEDS * 3; // 1 led = 3 channels

CRGB leds[NUM_LEDS];


ArtnetWifi artnet;
const int startUniverse = 0;

bool sendFrame = 1;
int previousDataLength = 0;

boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
  Serial.println("");
  Serial.println("Connecting to WiFi");


  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WLAN_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  sendFrame = 1;

  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
  }

  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < NUM_LEDS)
    {
      leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
  }
  previousDataLength = length;
  FastLED.show();
}

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin();
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);


  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  artnet.read();
}

*/
#include "control/ARTNET.h"
#include "control/HttpsAuthServer.h"
#include "config/HardwareConfig.h"



void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  Serial.println("---------- BEGIN DMX FRAME ----------");
  for (int i = 0; i < length; i++)
  {
    Serial.print(data[i]);
  }
  Serial.println("--------- END DMX FRAME ----------");
  
}


Artnet artnet;

HttpsAuthServer httpsServer;

void setup() {
  artnet.setup(onDmxFrame);
  httpsServer.begin(WLAN_SSID, WLAN_PASSWORD);
}

void loop() {
  artnet.loop();
  httpsServer.loop();
}


