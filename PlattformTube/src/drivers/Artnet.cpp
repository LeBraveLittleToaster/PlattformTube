#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>
#include <config/HardwareConfig.h>

#include "drivers/Artnet.h"

Artnet *Artnet::self_ = nullptr;

Artnet::Artnet(ConfigManager *config)
{
  this->config = config;
}

Artnet::~Artnet() {
  self_ = nullptr;
  artnet.stop();
}

boolean Artnet::ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i > 20)
    {
      state = false;
      break;
    }
    i++;
  }
  if (state)
  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WLAN_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

uint8_t Artnet::getChannel(int channel)
{
  // TODO: WHAT TO DO?!
  return -1;
}

void Artnet::onDmxStatic(uint16_t universe, uint16_t length,
                         uint8_t sequence, uint8_t *data)
{
  if (self_)
  {
    self_->onDmx(universe, length, sequence, data);
  }
}

void Artnet::onDmx(uint16_t universe, uint16_t length,
                   uint8_t sequence, const uint8_t *data)
{
  Serial.print("Universe: ");
  Serial.println(universe);
  for (uint8_t i = 0; i < length; i++)
  {
    dmxBuffer[i] = data[i];
    Serial.print(dmxBuffer[i]);
  }
}

void Artnet::begin()
{
  Serial.println("Begin starting Artnet instance");
  self_ = this;
  ConnectWifi();

  Serial.println("Starting UDP artnet endpoint");
  artnet.begin();

  Serial.println("Registering DMX Callback for artnet");
  artnet.setArtDmxCallback(onDmxStatic);
}

void Artnet::loop()
{
  artnet.read();
}

bool Artnet::readData()
{
  // Covered by loop
  return false;
}

uint8_t *Artnet::getBuffer()
{
  return dmxBuffer;
}

int Artnet::getBufferSize()
{
  return ARTNET_PACKET_SIZE;
}