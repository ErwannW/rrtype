/*
** EPITECH PROJECT, 2024
** Listen.cpp
** File description:
** Definition for Listen methods
*/

#include <ranges>
#include "Network/ABN/includes/Server.hpp"

using namespace ABN;

struct Connect
{
    uint16_t port;
};

bool Server::ProtectedCommandHandling(const packet_t& packet)
{
    if  (packet.header_.command_code_ == CONNECT) {
        const auto *conn = reinterpret_cast<const Connect *>(packet.body_.data());

        for (auto& client: clients_) {
            if (packet.header_.client_id_ == client.client_id_) {
                client.udp_port_ = conn->port;

                packet_t p{0};
                p.header_.client_id_ = client.client_id_;
                p.header_.body_size_ = 0;
                p.header_.command_code_ = CONNECT;

                std::lock_guard<std::mutex> lock(mutex_);

                if (verbose_)
                    ABN_DEBUG(SERVER_DEBUG, "[PROTECTEDCOMMAND]", "Client " << client.client_id_ << " connected")

                packets_.push_back(p);
                return true;
            }
        }
        return false;
    }
    return false;
}

void Server::TCPListenRoutine(const ClientStruct& client)
{
    while (tcp_server_.opened_) {
        try {
            packet_t packet{0};
            const std::size_t recv_size = client.client_tcp_socket_->read_some(asio::buffer(&packet, IPV4_MTU));

            if (recv_size < sizeof(header_t))
                continue;

            if (ProtectedCommandHandling(packet))
                continue;

            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[TCPLISTENROUTINE]", "Received " << recv_size << " bytes")

            if (!IsKnownClientID(packet.header_.client_id_))
                continue;

            std::lock_guard<std::mutex> lock(mutex_);
            packets_.push_back(packet);

        } catch (const std::exception& e) {
            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[TCPLISTENROUTINE]" , "Client " << client.client_id_ << " disconnected")

            const auto it = std::find_if(clients_.begin(), clients_.end(), [client](const ClientStruct& c1) {
                if (client.client_id_ == c1.client_id_) {
                    return true;
                }
                return false;
            });
            std::lock_guard<std::mutex> lock(mutex_);
            clients_.erase(it);

            header_t header{0};
            header.magic[0] = 'A';
            header.magic[1] = 'B';
            header.magic[2] = 'N';

            header.client_id_ = client.client_id_;
            header.body_size_ = 0;
            header.command_code_ = DISCONNECT;

            packet_t packet{0};
            packet.header_ = header;

            packets_.push_back(packet);
            return;
        }
    }
}

void Server::AcceptorRoutine()
{
    while (tcp_server_.opened_) {
        try {
            std::error_code ec;
            asio::ip::tcp::socket accepted_socket(tcp_server_.acceptor_.get_executor());

            tcp_server_.acceptor_.accept(accepted_socket, ec);
            if (!ec) {
                ClientStruct client;
                client.client_id_ = ++client_id_;
                std::shared_ptr<asio::ip::tcp::socket> socket = std::make_shared<asio::ip::tcp::socket>(std::move(accepted_socket));
                client.ip_address_ = socket->remote_endpoint().address().to_string();
                client.client_tcp_socket_ = socket;
                clients_.push_back(client);

                socket->send(asio::buffer(&client.client_id_, sizeof(uint16_t)));

                std::thread TCPListenRoutineThread([this, client]() mutable {
                    TCPListenRoutine(client);
                });
                TCPListenRoutineThread.detach();
                tcp_server_.listen_threads_.push_back(std::make_unique<std::thread>(std::move(TCPListenRoutineThread)));
            }
        } catch (const std::exception& e) {
            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[ACCEPTORROUTINE]" << ABN_EXCEPTION, e.what())
        }
    }
}

void Server::UDPListenRoutine()
{
    while (udp_server_.opened_) {
        try {
            packet_t packet{0};
            asio::ip::udp::endpoint sender_endpoint;
            const std::size_t recv_size = udp_server_.socket_.receive_from(asio::buffer(&packet, IPV4_MTU), sender_endpoint);

            if (recv_size < sizeof(header_t))
                continue;

            if (ProtectedCommandHandling(packet))
                continue;

            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[UDPLISTENROUTINE]", "Received " << recv_size << " bytes")

            if (!IsKnownClientID(packet.header_.client_id_))
                continue;

            std::lock_guard<std::mutex> guard(mutex_);
            packets_.push_back(packet);
        } catch (const std::exception& e) {
            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[UDPLISTENROUTINE]" << ABN_EXCEPTION, e.what())
        }
    }
}
