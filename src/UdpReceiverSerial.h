#pragma once

#include <HardwareSerial.h>
#include <BridgeUdp.h>
#include <cstdint>

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size>
struct UdpReceiverSerial : public DatagramReceiver<payload_buffer_size>
{
    void setup();

    void take(const Datagram<payload_buffer_size> &datagram) override;

private:
    uint8_t buffer[payload_buffer_size + 1]{ 0 };
};

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size> void UdpReceiverSerial<payload_buffer_size>::setup() {}

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size>
void UdpReceiverSerial<payload_buffer_size>::take(const Datagram<payload_buffer_size> &datagram)
{
    uint16_t bytes_buffered{ datagram.package.payload.bytes_buffered };

    memcpy(buffer, datagram.package.payload.data, bytes_buffered);
    buffer[datagram.package.payload.bytes_buffered + 1] = 0;

    Serial.printf("%s\n", buffer);
}
