#include <BridgeUdp.h>

constexpr const uint8_t MESSAGE_PAYLOAD_BUFFER = 255;
using UdpReceiverSerial_t = UdpReceiverSerial<MESSAGE_PAYLOAD_BUFFER>;
using BridgeUdp_t = BridgeUdp<MESSAGE_PAYLOAD_BUFFER>;
using SerialReaderUdp_t = SerialReaderUdp<MESSAGE_PAYLOAD_BUFFER>;

// multicast bridge joined network 239.0.0.1 with port 8622
BridgeUdp_t bridge{ { 239, 0, 0, 1 }, 8622, true };

// receives from bridge, sends to serial
UdpReceiverSerial_t datagram_receiver_serial;

// reads from serial, sends to bridge
SerialReaderUdp_t serial_reader;

bool isServerMode(){ /* ... */ };
bool isWifiConnected(){ /* ... */ };

void setup()
{
    if(isServerMode())
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("udp-bridge", "");
    }
    else
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin("udp-bridge", "");
    }

    while(!isWifiConnected()) {}

    datagram_receiver_serial.setup();

    bridge.setup(); // call each time wifi is reconnected
    bridge.setDatagramReceiver(&datagram_receiver_serial);

    serial_reader.setup();
    serial_reader.setDatagramReceiver(&bridge);
}

void loop()
{
    // read udp -> write to serial
    bridge.process();

    // read serial -> send UDP
    serial_reader.process();
}
