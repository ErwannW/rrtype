/*
** EPITECH PROJECT, 2024
** Is.cpp
** File description:
** Definition for Is methods
*/

#include "Server.hpp"

using namespace ABN;

bool Server::IsPortFree(uint16_t port) const
{
    std::error_code ec;
    asio::io_context context;
    asio::ip::tcp::acceptor acceptor(context);

    try {
        acceptor.open(asio::ip::tcp::v4(), ec) || acceptor.bind({ asio::ip::tcp::v4(), port }, ec);
        if (ec) {
            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[ISPORTFREE]" << ABN_ERROR, ec.message())
            return false;
        }
        acceptor.close();

        if (verbose_ && level_ == debug)
            ABN_DEBUG(SERVER_DEBUG, "[ISPORTFREE]", "Port free")

        return true;
    } catch (const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[ISPORTFREE]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Server::IsOpen() const
{
    const bool result = tcp_server_.opened_ && udp_server_.opened_;

    if (verbose_ && level_ == debug)
        ABN_DEBUG(SERVER_DEBUG, "[ISOPEN]", (result == true ? "Opened": "Not opened"))

    return result;
}

bool Server::IsKnownClientID(const uint16_t client_id) const
{
    for (const ClientStruct& client: clients_) {
        if (client.client_id_ == client_id) {
            if (verbose_ && level_ == debug)
                ABN_DEBUG(SERVER_DEBUG, "[ISKNOWNCLIENT]", "Known")
            return true;
        }
    }
    if (verbose_ && level_ == debug)
        ABN_DEBUG(SERVER_DEBUG, "[ISKNOWNCLIENT]", "Unknown")
    return false;
}
