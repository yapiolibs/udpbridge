#pragma once

#include <BridgeUdp.h>
#include <HardwareSerial.h>
#include <SerialLineReader.h>
#include <cstdint>

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size> struct SerialReaderUdp
{
    using Datagram_t = Datagram<payload_buffer_size>;
    using DatagramReceiver_t = DatagramReceiver<payload_buffer_size>;

    bool setDatagramReceiver(DatagramReceiver_t *datagram_receiver);

    void setup();

    bool process();

private:
    uint8_t buffer[payload_buffer_size + 1]{ 0 };

    SerialLineReader line_reader;
    DatagramReceiver_t *receiver{ nullptr };
};

// -------------------------------------------------------------------------------------------------

template <uint16_t payload_buffer_size> void SerialReaderUdp<payload_buffer_size>::setup() {}

// -------------------------------------------------------------------------------------------------

template <uint16_t package_payload_buffer_size>
bool SerialReaderUdp<package_payload_buffer_size>::process()
{
    bool has_processed_data{ false };

    if(line_reader.process())
        if(nullptr != receiver)
            while(line_reader.hasLine())
            {
                String line{ line_reader.getLine() };
                static Datagram_t datagram;
                if(line.length() <= sizeof(datagram.package.payload.data))
                {
                    uint16_t bytes_count{ static_cast<uint16_t>(line.length()) };
                    memcpy(datagram.package.payload.data, line.c_str(), bytes_count);
                    datagram.package.payload.bytes_buffered = bytes_count;
                    has_processed_data = (receiver->take(datagram)) ? true : has_processed_data;
                }
            }
    return has_processed_data;
}

// -------------------------------------------------------------------------------------------------

template <uint16_t package_payload_buffer_size>
bool SerialReaderUdp<package_payload_buffer_size>::setDatagramReceiver(DatagramReceiver_t *datagram_receiver)
{
    if(receiver != datagram_receiver)
    {
        receiver = datagram_receiver;
        return true;
    }

    receiver = datagram_receiver;
    return false;
}
