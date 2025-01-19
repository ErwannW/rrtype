//
// Created by ewicart on 05.12.24.
//

#include "Network/ABN/includes/Client.hpp"

using namespace ABN;

void Client::TCPListenRoutine()
{
    while (tcp_client_.connected_) {
        try {
            packet_t packet{0};
            const std::size_t recv_size = tcp_client_.socket_.read_some(asio::buffer(&packet, IPV4_MTU));

            if (recv_size < sizeof(header_t))
                continue;

            std::lock_guard<std::mutex> guard(mutex_);
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[TCPLISTEN]", "Received " << recv_size << " bytes from client " << packet.header_.client_id_)

            packets_.push_back(packet);
        } catch (const std::exception& e) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[TCPLISTEN]", "Server shutdown")
            Disconnect();
            return;
        }
    }
}

void Client::UDPListenRoutine()
{
    while (udp_client_.connected_) {
        try {
            packet_t packet{0};
            asio::ip::udp::endpoint server_endpoint;
            const std::size_t recv_size = udp_client_.socket_.receive_from(asio::buffer(&packet, IPV4_MTU), server_endpoint);

            if (recv_size < sizeof(header_t))
                continue;

            if (packet.header_.client_id_ == client_id_ && packet.header_.command_code_ == DISCONNECT) {
                if (verbose_)
                    ABN_DEBUG(CLIENT_DEBUG, "[UDPLISTEN]", "Not listening anymore")
                return;
            }

            std::lock_guard<std::mutex> guard(mutex_);
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[UDPLISTEN]", "Received " << recv_size << " bytes from server")

            packets_.push_back(packet);
        } catch (const std::exception& e) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[UDPLISTEN]" << ABN_EXCEPTION, e.what())
            return;
        }
    }
}
