//
// Created by ewicart on 05.12.24.
//

#include "Client.hpp"

using namespace ABN;

bool Client::IsPortOpen(const uint16_t port) const
{
    std::error_code ec;
    asio::io_context context;
    asio::ip::tcp::acceptor acceptor(context);

    try {
        acceptor.open(asio::ip::tcp::v4(), ec) || acceptor.bind({ asio::ip::tcp::v4(), port }, ec);
        if (ec) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[ISPORTOPEN]" << ABN_ERROR, ec.message())
            return false;
        }
        acceptor.close();

        if (verbose_ && level_ == debug)
            ABN_DEBUG(CLIENT_DEBUG, "[ISPORTOPEN]", "Port open")
        return true;
    } catch (const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[ISPORTOPEN]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Client::IsPortValid(const uint16_t port) const
{
    const bool result = port >= 1024 && port <= 65535;

    if (verbose_ && level_ == debug) {
        if (result)
            ABN_DEBUG(CLIENT_DEBUG, "[ISPORTVALD]", "Port valid")
        else
            ABN_DEBUG(CLIENT_DEBUG, "[ISPORTVALD]", "Port invalid")
    }
    return result;
}

bool Client::IsHostValid(const std::string& host) const
{
    try {
        std::error_code ec;
        asio::ip::make_address_v4(host, ec);

        if (ec) {
            if (verbose_)
                ABN_DEBUG(CLIENT_DEBUG, "[ISHOSTVALID]" << ABN_ERROR, ec.message())
            return false;
        }

        if (verbose_ && level_ == debug)
            ABN_DEBUG(CLIENT_DEBUG, "[ISHOSTVALID]", "Host valid")
        return true;
    } catch (const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(CLIENT_DEBUG, "[ISHOSTVALID]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Client::IsConnected() const
{
    const bool result = tcp_client_.connected_ && udp_client_.connected_;

    if (verbose_ && level_ == debug) {
        if (result)
            ABN_DEBUG(CLIENT_DEBUG, "[ISCONNECTED]", "Connected")
        else
            ABN_DEBUG(CLIENT_DEBUG, "[ISCONNECTED]", "Not connected")
    }
    return result;
}
