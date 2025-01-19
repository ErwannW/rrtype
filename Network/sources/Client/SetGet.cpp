//
// Created by ewicart on 05.12.24.
//

#include "Client.hpp"

using namespace ABN;

bool Client::SetTcpPort(const uint16_t port)
{
    if (IsConnected())
        return false;

    if (!IsPortValid(port))
        return false;

    tcp_client_.port_ = port;
    return true;
}

bool Client::SetUdpPort(const uint16_t port)
{
    if (IsConnected())
        return false;

    if (!IsPortValid(port))
        return false;

    udp_client_.port_ = port;
    return true;
}

bool Client::SetLocalUdpPort(const uint16_t port)
{
    if (IsConnected())
        return false;

    if (!IsPortValid(port) && !IsPortOpen(port))
        return false;

    udp_client_.local_port_ = port;
    return true;
}

bool Client::SetHost(const std::string &host)
{
    if (IsConnected())
        return false;

    if (!IsHostValid(host))
        return false;

    tcp_client_.host_ = host;
    udp_client_.host_ = host;
    return true;
}

void Client::GetPackets(std::vector<packet_t>& packets)
{
    if (packets_.empty())
        return;

    std::lock_guard<std::mutex> lock(mutex_);

    if (verbose_)
        ABN_DEBUG(CLIENT_DEBUG, "[GETPACKETS]", "Packets get")
    packets = std::move(packets_);
}

uint16_t Client::GetClientId() const
{
    return client_id_;
}

void Client::SetVerbose(const bool verbose)
{
    verbose_ = verbose;
}

void Client::SetVerboseLevel(const verbose_level level)
{
    level_ = level;
}
