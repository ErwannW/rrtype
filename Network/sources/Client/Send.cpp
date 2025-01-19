//
// Created by ewicart on 05.12.24.
//

// ReSharper disable CppDFAMemoryLeak
#include "Client.hpp"

using namespace ABN;

bool Client::Send(const void* data, const uint16_t body_size, const uint16_t command_code, const bool reliable)
{
    if (!IsConnected())
        return false;

    header_t header{0};
    header.magic[0] = 'A';
    header.magic[1] = 'B';
    header.magic[2] = 'N';

    header.body_size_ = body_size;
    header.command_code_ = command_code;
    header.client_id_ = client_id_;

    packet_t packet{0};
    packet.header_ = header;
    packet.body_ = Serialization(data, body_size);

    if (reliable)
        return TCPSend(&packet);
    return UDPSend(&packet);
}

bool Client::TCPSend(packet_t* packet)
{
    try {
        const uint16_t total_size = sizeof(header_t) + packet->header_.body_size_;

        if (total_size > IPV4_MTU)
            return false;

        const size_t bytes_sent = tcp_client_.socket_.send(asio::buffer(packet, total_size));

        if (bytes_sent != total_size)
            return false;

        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[TCPSEND]", "Send " << bytes_sent << " bytes")

        return true;
    } catch (const std::exception &e) {
        return false;
    }
}

bool Client::UDPSend(packet_t* packet)
{
    try {
        const uint16_t total_size = sizeof(header_t) + packet->header_.body_size_;

        if (total_size > IPV4_MTU)
            return false;

        const size_t bytes_sent = udp_client_.socket_.send_to(asio::buffer(packet, total_size), udp_client_.server_endpoint_);

        if (bytes_sent != total_size)
            return false;

        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[UDPSEND]", "Send " << bytes_sent << " bytes")

        return true;
    } catch (const std::exception &e) {
        return false;
    }
}
