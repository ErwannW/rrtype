/*
** EPITECH PROJECT, 2024
** Server.cpp
** File description:
** Definition for Server
*/

#include "Server.hpp"

using namespace ABN;


Server::Server(const uint16_t tcp_port, const uint16_t udp_port)
    : tcp_server_(tcp_port, verbose_, level_), udp_server_(udp_port, verbose_, level_) {}

Server::Server(const uint16_t tcp_port, const uint16_t udp_port, const bool verbose)
    : verbose_(verbose), tcp_server_(tcp_port, verbose_, basic), udp_server_(udp_port, verbose_, basic) {}

Server::Server(const uint16_t tcp_port, const uint16_t udp_port, const bool verbose, const verbose_level level)
    : verbose_(verbose), level_(level), tcp_server_(tcp_port, verbose_, level_), udp_server_(udp_port, verbose_, level_) {}

Server::~Server()
{
    if (IsOpen())
        Close();
}

bool Server::Open()
{
    if (IsOpen())
        return false;

    try {
        std::error_code ec;

        if (!tcp_server_.acceptor_.is_open()) {
            tcp_server_.acceptor_.open(asio::ip::tcp::v4(), ec);
            if (ec) {
                if (verbose_)
                    ABN_DEBUG(SERVER_DEBUG, "[OPEN]" << ABN_ERROR, ec.message())
                return false;
            }
        }
        tcp_server_.opened_ = true;

        if (!udp_server_.socket_.is_open()) {
            udp_server_.socket_.open(asio::ip::udp::v4(), ec);
            if (ec) {
                if (verbose_)
                    ABN_DEBUG(SERVER_DEBUG, "[OPEN]" << ABN_ERROR, ec.message())
                return false;
            }
        }
        udp_server_.opened_ = true;

        tcp_server_.acceptor_thread_ = std::thread([this]() { AcceptorRoutine(); });
        tcp_server_.acceptor_thread_.detach();

        udp_server_.listen_thread_ = std::thread([this]() { UDPListenRoutine(); });
        udp_server_.listen_thread_.detach();

        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[OPEN]", "Opened")

        return true;
    } catch ([[maybe_unused]] const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[OPEN]" << ABN_EXCEPTION, e.what())
        return false;
    }
}

bool Server::Close()
{
    if (!IsOpen())
        return true;

    try {
        std::error_code ec;

        tcp_server_.acceptor_.close(ec);
        if (ec) {
            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[CLOSE]" << ABN_ERROR, ec.message())
            return false;
        }
        tcp_server_.opened_ = false;

        for (auto& client_thread: tcp_server_.listen_threads_)
            if (client_thread->joinable())
                client_thread->join();

        udp_server_.socket_.close(ec);
        if (ec) {
            if (verbose_)
                ABN_DEBUG(SERVER_DEBUG, "[CLOSE]" << ABN_ERROR, ec.message())
            return false;
        }
        udp_server_.opened_ = false;

        clients_.clear();

        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[CLOSE]", "Closed")

        return true;
    } catch ([[maybe_unused]] const std::exception& e) {
        if (verbose_)
            ABN_DEBUG(SERVER_DEBUG, "[CLOSE]" << ABN_EXCEPTION, e.what())
        return false;
    }
}
