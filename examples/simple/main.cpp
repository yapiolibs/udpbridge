#include <BridgeUdp.h>


constexpr const uint8_t MESSAGE_PAYLOAD_BUFFER = 255;
using BridgeUdp_t = BridgeUdp<MESSAGE_PAYLOAD_BUFFER>;
using UdpReceiverSerial_t = UdpReceiverSerial<MESSAGE_PAYLOAD_BUFFER>;

// 239.0.0.1:8622, multicast
BridgeUdp_t bridge{{239, 0, 0, 1}, 8622, true };

// datagram receiver
UdpReceiverSerial_t datagram_receiver_serial;

// true  ... AP wifi mode
// false ... STA wifi mode
bool server_mode = false;

bool isWifiConnected();
bool serialReadLine();

void setup()
{
    if(server_mode)
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("udp-bridge", "");
    }
    else
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin("udp-bridge", "");
    }

    while (!isWifiConnected()){}
    
    datagram_receiver_serial.setup();
    bridge.setup(); // call each time wifi is reconnected
    bridge.setDatagramReceiver(&datagram_receiver_serial);
}

void loop()
{
    // read udp -> send to serial
    bridge.process()
    
    // read serial -> write to udp
    String line = serialReadLine();
    if (line.length() > 0)
    {
        BridgeUdp_t::Datagram_t datagram;
        if(line.length() <= BridgeUdp_t::PACKAGE_PAYLOAD_BUFFER_SIZE)
        {
            memcpy(datagram.package.payload.data, line.c_str(), line.length());
            datagram.package.payload.bytes_buffered = static_cast<uint8_t>(line.length());
            bridge.send(datagram);
        }
    }
}

bool isWifiConnected()
{
    if(server_mode && WiFi.softAPgetStationNum() > 0)
    {
        return true;
    }

    if(!server_mode && WiFi.isConnected())
    {
        return true;
    }

    return false;
}

String serialReadLine()
{
    // returns "" or full line, non blocking
}
