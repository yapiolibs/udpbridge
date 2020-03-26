#pragma once

#include <HardwareSerial.h>
#include <BridgeUdp.h>
#include <cstdint>

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size>
struct UdpReceiverSerial : public DatagramReceiver<payload_buffer_size>
{
    void setup();

    bool take(Datagram<payload_buffer_size> &datagram) override;

private:
    uint8_t buffer[payload_buffer_size + 1]{ 0 };
};

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size> void UdpReceiverSerial<payload_buffer_size>::setup() {}

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size>
bool UdpReceiverSerial<payload_buffer_size>::take(Datagram<payload_buffer_size> &datagram)
{
    memcpy(&buffer, datagram.package.payload.data, datagram.package.payload.bytes_buffered);
    buffer[datagram.package.payload.bytes_buffered] = 0;
    Serial.printf("%s", buffer);
    return true;
}
