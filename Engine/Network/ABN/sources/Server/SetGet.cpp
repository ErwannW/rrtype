/*
** EPITECH PROJECT, 2024
** SetGet.cpp
** File description:
** Definition for getters and setters
*/

#include "Network/ABN/includes/Server.hpp"

using namespace ABN;

bool Server::SetTcpPort(const uint16_t port)
{
    if (IsOpen())
        return false;

    if (!IsPortFree(port))
        return false;

    try {
        tcp_server_.port_ = port;
        tcp_server_.server_endpoint_ = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), tcp_server_.port_);
        tcp_server_.acceptor_ = asio::ip::tcp::acceptor(tcp_server_.io_context_, tcp_server_.server_endpoint_);

        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[SETTCPPORT]", "Set")

        return true;
    } catch ([[maybe_unused]] const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[SETUDPPORT]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Server::SetUdpPort(const uint16_t port)
{
    if (IsOpen())
        return false;

    if (!IsPortFree(port))
        return false;

    try {
        udp_server_.port_ = port;
        udp_server_.local_endpoint_ = asio::ip::udp::endpoint(asio::ip::udp::v4(), udp_server_.port_);
        udp_server_.socket_ = asio::ip::udp::socket(udp_server_.io_context_, udp_server_.local_endpoint_);

        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[SETUDPPORT]", "Set")

        return true;
    } catch ([[maybe_unused]] const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[SETUDPPORT]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

void Server::GetPackets(std::vector<packet_t>& packets)
{
    if (packets_.empty())
        return;

    std::lock_guard<std::mutex> lock(mutex_);
    if (verbose_)
        ABN_DEBUG(SERVER_DEBUG, "[GETPACKETS]", "Packets get")
    packets = std::move(packets_);
}

void Server::SetVerbose(bool verbose)
{
    verbose_ = verbose;
}

void Server::SetVerboseLevel(verbose_level level)
{
    level_ = level;
}
