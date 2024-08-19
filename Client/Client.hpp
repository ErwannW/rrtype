/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** Client.cppm
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <memory>
#include <string>
#include <cstdint>
#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#define SUCCESS 0
#define FAIL 84
#define MAX_PAYLOAD_DATA 508

namespace UDP {
    namespace ClientException {
        class ptrException : public std::exception {
            private:
                const std::string message_;
            public:
                explicit inline ptrException(const std::string &message): message_("Pointer Exception: " + message) {}

                [[nodiscard]] const char *what() const noexcept override { return (message_.c_str()); }
        };

        class portException : public std::exception {
        private:
            const std::string message_;
        public:
            explicit inline portException(const std::string &message): message_("Port Exception: " + message) {}

            [[nodiscard]] const char *what() const noexcept override { return (message_.c_str()); }
        };

        class hostException : public std::exception {
        private:
            const std::string message_;
        public:
            explicit inline hostException(const std::string &message): message_("Host Exception: " + message) {}

            [[nodiscard]] const char *what() const noexcept override { return (message_.c_str()); }
        };
    }
}

#define CLIENT_UUID_COUT std::cout << "[" << uuid_.substr(0,5) << "] "
#define CLIENT_UUID_CERR std::cerr << "[" << uuid_.substr(0,5) << "] "

struct header_t {

};

namespace UDP {
    template<typename M>
    class Client {
        private:
            uint16_t port_;
            std::string host_;
            bool verbose_;
            std::shared_ptr<M> header_ptr_;

            std::atomic<bool> stopFlag_ = false;
            bool connected_ = false;
            boost::asio::ip::address address_;

            boost::asio::io_context io_context_;
            boost::asio::ip::udp::resolver resolver_;
            boost::asio::ip::udp::resolver::results_type endpoints_;
            boost::asio::ip::udp::socket socket_;

            static bool isHostValid(const std::string &host);
            static bool isPortValid(uint16_t port);

            const std::string uuid_ = boost::uuids::to_string(boost::uuids::random_generator()());
            std::thread thread_;

            void listenRoutine(void);
        public:
            Client(uint16_t port, const std::string &host, bool verbose, std::shared_ptr<M> header_ptr);
            ~Client();

            bool setPort(uint16_t port);
            bool setHost(const std::string &host);
            bool setHeaderPtr(std::shared_ptr<M> header_ptr);

            void setVerbose(bool verbose);

            [[nodiscard]] uint16_t getPort(void) const;
            [[nodiscard]] const std::string &getHost(void) const;
            [[nodiscard]] bool getVerbose(void) const;

            bool listen(void);
            bool disconnectFromServer(void);
    };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// DEFINITION //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename M>
bool UDP::Client<M>::isHostValid(const std::string &host)
{
    if (host == "localhost")
        return (true);

    boost::system::error_code ec;
    boost::asio::ip::make_address(host, ec);

    return (!ec);
}

template<typename M>
bool UDP::Client<M>::isPortValid(uint16_t port)
{
    if (port < 1024 || port > 65535)
        return (false);
    return (true);
}

template<typename M>
UDP::Client<M>::Client(uint16_t port, const std::string &host, const bool verbose, std::shared_ptr<M> header_ptr):
port_(port), host_(host), verbose_(verbose), header_ptr_(header_ptr)
, resolver_(io_context_), socket_(io_context_)
{
    try {
        if (!isPortValid(port))
            throw ClientException::portException("Invalid port: " + std::to_string(port));
        if (!isHostValid(host))
            throw ClientException::hostException("Invalid host: " + host);
        if (host == "localhost")
            address_ = boost::asio::ip::make_address_v4("127.0.0.1");
        else
            address_  = boost::asio::ip::make_address_v4(host);
        if (header_ptr == nullptr)
            throw ClientException::ptrException("Null pointer");
        if (verbose)
            CLIENT_UUID_COUT << "Client created with uuid: '" << uuid_ << "'" << std::endl;
    } catch (const ClientException::ptrException &e) {
        CLIENT_UUID_CERR << e.what() << std::endl;
        return;
    } catch (const ClientException::portException &e) {
        CLIENT_UUID_CERR << e.what() << std::endl;
        return;
    } catch (const ClientException::hostException &e) {
        CLIENT_UUID_CERR << e.what() << std::endl;
        return;
    } catch (const std::exception &e) {
        CLIENT_UUID_CERR << e.what() << std::endl;
        return;
    }
}

template<typename M>
UDP::Client<M>::~Client()
{
    if (connected_) {
        if (verbose_)
            CLIENT_UUID_COUT << "Client connected to a server, trying to disconnect from it..." << std::endl;
        if (!disconnectFromServer()) {
            // ReSharper disable once CppDFAUnreachableCode
            if (verbose_) {
                CLIENT_UUID_COUT << "Failed to disconnect from server" << std::endl;
                return;
            }
            CLIENT_UUID_COUT << "Client disconneted from server" << std::endl;
        }
    }
    CLIENT_UUID_COUT << "Client deleted" << std::endl;
}

template<typename M>
bool UDP::Client<M>::setPort(uint16_t port)
{
    if (connected_) {
        if (verbose_)
            CLIENT_UUID_COUT << "Error port setter: Client connected to a server" << std::endl;
        return (false);
    }
    if (!isPortValid(port)) {
        if (verbose_)
            CLIENT_UUID_COUT << "Error port setter: Invalid port" << std::endl;
        return (false);
    }

    port_ = port;
    if (verbose_)
        CLIENT_UUID_COUT << "New port set" << std::endl;
    return (true);
}

template<typename M>
bool UDP::Client<M>::setHost(const std::string &host)
{
    if (connected_) {
        if (verbose_)
            CLIENT_UUID_COUT << "Error host setter: Client connected to a server" << std::endl;
        return (false);
    }
    if (!isHostValid(host)) {
        if (verbose_)
            CLIENT_UUID_COUT << "Error host setter: Invalid host" << std::endl;
        return (false);
    }

    host_ = host;
    if (verbose_)
        CLIENT_UUID_COUT << "New host set" << std::endl;
    return (true);
}

template<typename M>
bool UDP::Client<M>::setHeaderPtr(std::shared_ptr<M> header_ptr)
{
    if (connected_) {
        if (verbose_)
            CLIENT_UUID_COUT << "Error pointer setter: Client connected to a server" << std::endl;
        return (false);
    }
    if (header_ptr == nullptr) {
        if (verbose_)
            CLIENT_UUID_COUT << "Error pointer setter: Null pointer" << std::endl;
        return (false);
    }

    header_ptr_ = header_ptr;
    if (verbose_)
        CLIENT_UUID_COUT << "New pointer set" << std::endl;
    return (true);
}

template<typename M>
void UDP::Client<M>::setVerbose(bool verbose)
{
    verbose_ = verbose;
}

template<typename M>
uint16_t UDP::Client<M>::getPort() const
{
    return (port_);
}

template<typename M>
const std::string & UDP::Client<M>::getHost() const
{
    return (host_);
}

template<typename M>
bool UDP::Client<M>::getVerbose() const
{
    return (verbose_);
}

template<typename M>
void UDP::Client<M>::listenRoutine()
{
    while (true) {
        boost::asio::ip::udp::endpoint remote_endpoint_;
        header_ptr_->resize(MAX_PAYLOAD_DATA);
        socket_.receive_from(boost::asio::buffer(*header_ptr_), remote_endpoint_);
    }
}

template<typename M>
bool UDP::Client<M>::listen()
{
    try {
        connected_ = true;
        boost::system::error_code ec;
        endpoints_ = resolver_.resolve(boost::asio::ip::udp::v4(), host_, std::to_string(port_), ec);

        if (ec) {
            if (verbose_)
                CLIENT_UUID_COUT << "Error connecting: Resolve: " << ec.message() << std::endl;
            return (false);
        }

        socket_.open(boost::asio::ip::udp::v4(), ec);
        if (ec) {
            if (verbose_)
                CLIENT_UUID_COUT << "Error connecting: Open: " << ec.message() << std::endl;
            return (false);
        }

        socket_.send_to(boost::asio::buffer(std::string("hello from client")), *endpoints_.begin());

        thread_ = std::thread([this]() { listenRoutine(); });


    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return (true);
}

template<typename M>
bool UDP::Client<M>::disconnectFromServer()
{
    if (!connected_) {
        if (verbose_)
            CLIENT_UUID_COUT << "Error disconnecting: Not connected to a server" << std::endl;
        return (false);
    }
    connected_ = false;
    if (verbose_)
        CLIENT_UUID_COUT << "Disconnected from server" << std::endl;
    return (true);
}

#endif /* CLIENT_HPP */
