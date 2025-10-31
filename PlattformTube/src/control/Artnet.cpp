#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>
#include <config/HardwareConfig.h>

#include "control/Artnet.h"


boolean Artnet::ConnectWifi(void)
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

void Artnet::setup(void (*onDmxFramePtr)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data))
{
  ConnectWifi();
  artnet.begin();

  artnet.setArtDmxCallback(onDmxFramePtr);
}

void Artnet::loop()
{
  artnet.read();
}