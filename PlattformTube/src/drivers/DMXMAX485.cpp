#include "drivers/DMXMAX485.h"
#include "config/HardwareConfig.h" // For DMX pin definitions

DMXMAX485::DMXMAX485(
    dmx_port_t dmxPort,
    int rxPin,
    int txPin,
    int enPin
) : dmxPort(dmxPort), rxPin(rxPin), txPin(txPin), enPin(enPin) {}

void DMXMAX485::begin() {
    dmx_config_t config = DMX_CONFIG_DEFAULT;
    dmx_personality_t personalities[] = {
        {1, "Default Personality"}
    };
    int personality_count = 1;

    dmx_driver_install(dmxPort, &config, personalities, personality_count);

    // Pins are defined in hardwareconfig.h or passed to constructor
    dmx_set_pin(dmxPort, DMX_TX_PIN, DMX_RX_PIN, DMX_EN_PIN);
}

bool DMXMAX485::readData() {
    dmx_packet_t packet;
    if (dmx_receive(dmxPort, &packet, DMX_TIMEOUT_TICK)) {
        if (!packet.err) {
            dmx_read(dmxPort, dmxBuffer, packet.size);
            dmxIsConnected = true;
            return true;
        } else {
            dmxIsConnected = false;
        }
    }
    return false;
}

const uint8_t* DMXMAX485::getBuffer() const {
    return dmxBuffer;
}

uint8_t DMXMAX485::getChannel(int channel) {
    if (channel >= 0 && channel < DMX_BUFFER_SIZE) {
        return dmxBuffer[channel];
    }
    return 0;
}

DMXMAX485::~DMXMAX485() {
    dmx_driver_delete(dmxPort);
}