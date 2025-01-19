/*
** EPITECH PROJECT, 2024
** Send.cpp
** File description:
** Definition for Send methods
*/

#include "Network/ABN/includes/Server.hpp"

using namespace ABN;

bool Server::TCPSend(packet_t* packet, const std::shared_ptr<asio::ip::tcp::socket>& socket)
{
    try {
        const uint16_t total_size = sizeof(header_t) + packet->header_.body_size_;
        const size_t bytes_send = socket->send(asio::buffer(packet, total_size));

        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[TCPSEND]", "Send " << bytes_send << " bytes")

        return true;
    } catch (const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[TCPSEND]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Server::UDPSend(packet_t* packet, const std::string& ip_addr, const uint16_t port)
{
    try {
        std::error_code ec;
        asio::ip::udp::resolver resolver(udp_server_.io_context_);
        const auto remote =  *resolver.resolve(asio::ip::udp::v4(), ip_addr, std::to_string(port), ec);

        if (ec) {
            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[UDPSEND]" << ABN_ERROR, ec.message())
            return false;
        }

        const uint16_t total_size = sizeof(header_t) + packet->header_.body_size_;
        const size_t bytes_send = udp_server_.socket_.send_to(asio::buffer(packet, total_size), remote);

        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[UDPSEND]", "Send " << bytes_send << " bytes")

        return bytes_send == total_size;
    } catch (const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[UDPSEND]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Server::Send(const void* data, const uint16_t body_size, const uint16_t command_code, const bool reliable)
{
    if (!IsOpen())
        return false;

    if (clients_.empty())
        return false;

    header_t header{0};
    header.command_code_ = command_code;
    header.body_size_ = body_size;
    header.client_id_ = SERVER_ID;

    packet_t packet{0};
    packet.header_ = header;
    packet.body_ = Serialization(data, body_size);

    for (const auto& client : clients_) {
        if (reliable) {
            TCPSend(&packet, client.client_tcp_socket_);
        } else {
            if (client.ip_address_.empty() || client.udp_port_ == 0)
                continue;
            UDPSend(&packet, client.ip_address_, client.udp_port_);
        }
    }
    return true;
}

bool Server::Send(const void* data, const uint16_t body_size, const uint16_t command_code, const bool reliable, const uint16_t client_id)
{
    if (!IsOpen())
        return false;

    if (clients_.empty())
        return false;

    for (const auto& client : clients_) {
        if (client.client_id_ == client_id) {
            header_t header{0};
            header.command_code_ = command_code;
            header.client_id_ = SERVER_ID;
            header.body_size_ = body_size;

            packet_t packet{0};
            packet.header_ = header;
            packet.body_ = Serialization(data, body_size);

            if (reliable) {
                return TCPSend(&packet, client.client_tcp_socket_);
            } else {
                if (client.ip_address_.empty() || client.udp_port_ == 0) {
                    if (verbose_)
                        ABN_DEBUG(SERVER_DEBUG, "[SEND]" << ABN_ERROR, "Missing udp address or port")
                    return false;
                }
                return UDPSend(&packet, client.ip_address_, client.udp_port_);
            }
        }
    }
    if (verbose_)
        ABN_DEBUG(SERVER_DEBUG, "[SEND]" << ABN_ERROR, "Client not found")
    return false;
}
