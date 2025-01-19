//
// Created by ewicart on 04.12.24.
//

#include "Client.hpp"

using namespace ABN;

Client::Client(const uint16_t tcp_port, const uint16_t udp_port, const uint16_t local_udp_port)
    : tcp_client_(tcp_port, verbose_, level_), udp_client_(udp_port, local_udp_port,verbose_, level_) {}

Client::Client(const uint16_t tcp_port, const uint16_t udp_port, const uint16_t local_udp_port, const bool verbose)
    : verbose_(verbose), tcp_client_(tcp_port, verbose_, level_), udp_client_(udp_port, local_udp_port,verbose_, level_) {}

Client::Client(const uint16_t tcp_port, const uint16_t udp_port, const uint16_t local_udp_port, const bool verbose, verbose_level level)
    : verbose_(verbose), level_(level), tcp_client_(tcp_port, verbose_, level_), udp_client_(udp_port, local_udp_port,verbose_, level_) {}

Client::Client(const uint16_t tcp_port, const uint16_t udp_port, const uint16_t local_udp_port, const std::string &host)
    : tcp_client_(tcp_port, host, verbose_, level_), udp_client_(udp_port, local_udp_port,host, verbose_, level_) {}

Client::Client(const uint16_t tcp_port, const uint16_t udp_port, const uint16_t local_udp_port, const std::string &host, const bool verbose)
    : verbose_(verbose), tcp_client_(tcp_port, host, verbose_, level_), udp_client_(udp_port, local_udp_port,host, verbose_, level_) {}

Client::Client(const uint16_t tcp_port, const uint16_t udp_port, const uint16_t local_udp_port, const std::string &host, const bool verbose, const verbose_level level)
    : verbose_(verbose), level_(level), tcp_client_(tcp_port, host, verbose_, level_), udp_client_(udp_port, local_udp_port, host, verbose_, level_) {}

Client::~Client()
{
    if (IsConnected())
        Disconnect();
}

bool Client::Connect()
{
    if (!IsPortOpen(udp_client_.local_port_)) {
        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[CONNECT]", "Local port is already used")
    }
    try {
        std::error_code ec;
        tcp_client_.server_endpoint_ = asio::ip::tcp::endpoint(asio::ip::make_address_v4(tcp_client_.host_), tcp_client_.port_);

        tcp_client_.socket_.connect(tcp_client_.server_endpoint_, ec);
        if (ec) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[CONNECT]" << ABN_ERROR, ec.message())
            return false;
        }

        uint16_t client_id = 0;
        tcp_client_.socket_.read_some(asio::buffer(&client_id, sizeof(uint16_t)), ec);
        if (ec || client_id == 0)
            return false;
        client_id_ = client_id;
        tcp_client_.connected_ = true;

        header_t header{0};
        header.magic[0] = 'A';
        header.magic[1] = 'B';
        header.magic[2] = 'N';

        header.client_id_ = client_id;
        header.command_code_ = CONNECT;
        header.body_size_ = sizeof(uint16_t);

        packet_t packet{0};
        packet.header_ = header;
        packet.body_ = Serialization(&udp_client_.local_port_, sizeof(uint16_t));
        const uint16_t packet_size = header.body_size_ + sizeof(header_t);
        const uint16_t send_size = tcp_client_.socket_.send(asio::buffer(&packet, packet_size));

        if (send_size != packet_size) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[CONNECT]", "Fail to send all info, only " << send_size << " bytes and not " << packet_size);
            return false;
        }

        if (verbose_ && level_ == debug)
            ABN_DEBUG(CLIENT_DEBUG, "[CONNECT]", "Send client's information")

        udp_client_.server_endpoint_ = *udp_client_.resolver_.resolve(asio::ip::udp::v4(), udp_client_.host_, std::to_string(udp_client_.port_), ec);
        if (ec) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[CONNECT]" << ABN_ERROR, ec.message())
            return false;
        }

        if (!udp_client_.socket_.is_open()) {
            udp_client_.socket_.open(asio::ip::udp::v4(),  ec);
            if (ec) {
                if (verbose_)
                    ABN_DEBUG(CLIENT_DEBUG, "[CONNECT]" << ABN_ERROR, ec.message())
                return false;
            }
        }
        udp_client_.connected_ = true;

        tcp_client_.listen_thread_ = std::thread([this]() { TCPListenRoutine(); });
        tcp_client_.listen_thread_.detach();

        udp_client_.listen_thread_ = std::thread([this]() { UDPListenRoutine(); });
        udp_client_.listen_thread_.detach();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[CONNECT]", "Connected")
        return true;
    } catch (const std::exception &e) {
        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[DISCONNECT]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Client::Disconnect()
{
    if (!IsConnected())
        return true;

    try {
        std::error_code ec;

        tcp_client_.socket_.close(ec);
        if (ec) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[DISCONNECT]" << ABN_ERROR, ec.message())
            return false;
        }
        tcp_client_.connected_ = false;
        if (tcp_client_.listen_thread_.joinable())
            tcp_client_.listen_thread_.join();

        udp_client_.socket_.close();
        if (ec) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[DISCONNECT]" << ABN_ERROR, ec.message())
            return false;
        }
        udp_client_.connected_ = false;
        if (udp_client_.listen_thread_.joinable())
            udp_client_.listen_thread_.join();

        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[DISCONNECT]", "Disconnected")
        return true;
    } catch (const std::exception &e) {
        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[DISCONNECT]" << ABN_EXCEPTION, e.what())
        return false;
    }
}
