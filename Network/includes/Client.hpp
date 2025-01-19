/*
** EPITECH PROJECT, 2024
** Client.cpp
** File description:
** Definition for Client
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "Misc.hpp"

namespace ABN
{
    /**
     * @struct TCPClient
     * @brief Structure to represent a TCP client.
     *
     * This structure encapsulates the necessary data and functions
     * for setting up a TCP client using ASIO, including connection details
     * and communication over the TCP protocol.
     */
    struct TCPClient
    {
        /**
         * @var connected_
         * @brief Flag indicating if the client is connected to the server.
         */
        bool connected_ = false;

        /**
         * @var port_
         * @brief Port number to connect to on the server.
         */
        uint16_t port_ = 8080;

        /**
         * @var host_
         * @brief Host IP address or hostname of the server.
         */
        std::string host_ = "127.0.0.1";

        /**
         * @var io_context_
         * @brief The ASIO I/O context for managing asynchronous operations.
         */
        asio::io_context io_context_ = asio::io_context();

        /**
         * @var server_endpoint_
         * @brief The server's endpoint (IP address and port) to which the client connects.
         */
        asio::ip::tcp::endpoint server_endpoint_;

        /**
         * @var socket_
         * @brief The TCP socket used to communicate with the server.
         */
        asio::ip::tcp::socket socket_;

        /**
         * @var listen_thread_
         * @brief The thread used for listening to server responses.
         */
        std::thread listen_thread_;

        /**
         * @brief Constructor to initialize the TCP client with optional verbosity.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        explicit TCPClient(const bool verbose = false, const verbose_level level = basic)
            : socket_(io_context_)
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[TCPCLIENT]", "Created")
        };

        /**
         * @brief Constructor to initialize the TCP client with a specific port and optional verbosity.
         * @param port The port number to connect to on the server.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        TCPClient(const uint16_t port, const bool verbose, const verbose_level level)
            : port_(port), socket_(io_context_)
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[TCPCLIENT]", "Created")
        };

        /**
         * @brief Constructor to initialize the TCP client with a specific host and optional verbosity.
         * @param host The host address (IP or hostname) of the server.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        TCPClient(const std::string& host, const bool verbose, const verbose_level level)
            : host_(host), socket_(io_context_)
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[TCPCLIENT]", "Created")
        };

        /**
         * @brief Constructor to initialize the TCP client with a specific port, host, and optional verbosity.
         * @param port The port number to connect to on the server.
         * @param host The host address (IP or hostname) of the server.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        TCPClient(const uint16_t port, const std::string& host, const bool verbose, const verbose_level level)
            : port_(port), host_(host), socket_(io_context_)
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[TCPCLIENT]", "Created")
        };
    };

    /**
     * @struct UDPClient
     * @brief Structure to represent a UDP client.
     *
     * This structure encapsulates the necessary data and functions
     * for setting up a UDP client using ASIO, including connection details
     * and communication over the UDP protocol.
     */
    struct UDPClient
    {
        /**
         * @var connected_
         * @brief Flag indicating if the client is connected to the server.
         */
        bool connected_ = false;

        /**
         * @var port_
         * @brief Port number to connect to on the server.
         */
        uint16_t port_ = 8081;

        /**
         * @var local_port_
         * @brief Local port number used by the client.
         */
        uint16_t local_port_ = 12345;

        /**
         * @var host_
         * @brief Host IP address or hostname of the server.
         */
        std::string host_ = "127.0.0.1";

        /**
         * @var io_context_
         * @brief The ASIO I/O context for managing asynchronous operations.
         */
        asio::io_context io_context_ = asio::io_context();

        /**
         * @var resolver_
         * @brief The ASIO resolver used to resolve the server's endpoint.
         */
        asio::ip::udp::resolver resolver_;

        /**
         * @var server_endpoint_
         * @brief The server's endpoint (IP address and port) to which the client sends/receives UDP packets.
         */
        asio::ip::udp::endpoint server_endpoint_;

        /**
         * @var socket_
         * @brief The UDP socket used to communicate with the server.
         */
        asio::ip::udp::socket socket_;

        /**
         * @var listen_thread_
         * @brief The thread used for listening to server responses.
         */
        std::thread listen_thread_;

        /**
         * @brief Constructor to initialize the UDP client with optional verbosity.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        explicit UDPClient(const bool verbose = false, const verbose_level level = basic)
            : resolver_(io_context_), socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), local_port_))
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[UDPCLIENT]", "Created")
        };

        /**
         * @brief Constructor to initialize the UDP client with a specific port, local port, and optional verbosity.
         * @param port The port number to connect to on the server.
         * @param local_port The local port number used by the client.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        UDPClient(const uint16_t port, const uint16_t local_port, const bool verbose, const verbose_level level)
            : port_(port), local_port_(local_port), resolver_(io_context_), socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), local_port_))
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[UDPCLIENT]", "Created")
        };

        /**
         * @brief Constructor to initialize the UDP client with a specific host and optional verbosity.
         * @param host The host address (IP or hostname) of the server.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        UDPClient(const std::string& host, const bool verbose, const verbose_level level)
            : host_(host), resolver_(io_context_), socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), local_port_))
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[UDPCLIENT]", "Created")
        };

        /**
         * @brief Constructor to initialize the UDP client with a specific port, local port, host, and optional verbosity.
         * @param port The port number to connect to on the server.
         * @param local_port The local port number used by the client.
         * @param host The host address (IP or hostname) of the server.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The verbosity level (basic, detailed).
         */
        UDPClient(const uint16_t port, const uint16_t local_port, const std::string& host, const bool verbose, const verbose_level level)
            : port_(port), local_port_(local_port), host_(host), resolver_(io_context_), socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), local_port_))
        {
            if (verbose)
                ABN_DEBUG(CLIENT_DEBUG, "[UDPCLIENT]", "Created")
        };
    };

    /**
     * @class Client
     * @brief Class representing a client that can communicate using both TCP and UDP protocols.
     *
     * This class provides methods to connect to a server, send data, and listen for responses.
     * It supports both reliable (TCP) and unreliable (UDP) communication, allowing the client to
     * choose the appropriate protocol for each task.
     */
    class Client {
        public:
            /**
             * @brief Default constructor for the client.
             */
            Client() = default;

            /**
             * @brief Constructor to initialize the client with specific TCP, UDP ports, and local UDP port.
             * @param tcp_port The port for the TCP connection.
             * @param udp_port The port for the UDP connection.
             * @param local_udp_port The local UDP port used by the client.
             */
            Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port);

            /**
             * @brief Constructor to initialize the client with specific ports and optional verbosity.
             * @param tcp_port The port for the TCP connection.
             * @param udp_port The port for the UDP connection.
             * @param local_udp_port The local UDP port used by the client.
             * @param verbose A boolean to enable or disable verbose logging.
             */
            Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, bool verbose);

            /**
             * @brief Constructor to initialize the client with specific ports and verbosity level.
             * @param tcp_port The port for the TCP connection.
             * @param udp_port The port for the UDP connection.
             * @param local_udp_port The local UDP port used by the client.
             * @param verbose A boolean to enable or disable verbose logging.
             * @param level The verbosity level (basic, detailed).
             */
            Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, bool verbose, verbose_level level);

            /**
             * @brief Constructor to initialize the client with specific ports and a host.
             * @param tcp_port The port for the TCP connection.
             * @param udp_port The port for the UDP connection.
             * @param local_udp_port The local UDP port used by the client.
             * @param host The host IP address or hostname.
             */
            Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, const std::string& host);

            /**
             * @brief Constructor to initialize the client with specific ports, a host, and verbosity.
             * @param tcp_port The port for the TCP connection.
             * @param udp_port The port for the UDP connection.
             * @param local_udp_port The local UDP port used by the client.
             * @param host The host IP address or hostname.
             * @param verbose A boolean to enable or disable verbose logging.
             */
            Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, const std::string& host, bool verbose);

            /**
             * @brief Constructor to initialize the client with specific ports, a host, and verbosity level.
             * @param tcp_port The port for the TCP connection.
             * @param udp_port The port for the UDP connection.
             * @param local_udp_port The local UDP port used by the client.
             * @param host The host IP address or hostname.
             * @param verbose A boolean to enable or disable verbose logging.
             * @param level The verbosity level (basic, detailed).
             */
            Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, const std::string& host, bool verbose, verbose_level level);

            /**
             * @brief Destructor for the client class.
             */
            ~Client();

            /**
             * @brief Connects the client to the server.
             * @return True if the connection was successful, false otherwise.
             */
            bool Connect();

            /**
             * @brief Disconnects the client from the server.
             * @return True if the disconnection was successful, false otherwise.
             */
            bool Disconnect();

            /**
             * @brief Sends data to the server.
             * @param data A pointer to the data to send.
             * @param body_size The size of the data to send.
             * @param command_code The command code associated with the data.
             * @param reliable A boolean indicating if the communication should be reliable (TCP) or unreliable (UDP).
             * @return True if the data was sent successfully, false otherwise.
             */
            bool Send(const void* data, uint16_t body_size, uint16_t command_code, bool reliable);

            /**
             * @brief Retrieves all the received packets.
             * @param packets A vector to store the retrieved packets.
             */
            void GetPackets(std::vector<packet_t>& packets);

            /**
             * @brief Gets the client ID.
             * @return The client ID.
             */
            [[nodiscard]] uint16_t GetClientId() const;

            /**
             * @brief Sets whether verbose logging is enabled.
             * @param verbose A boolean to enable or disable verbose logging.
             */
            void SetVerbose(bool verbose);

            /**
             * @brief Sets the verbosity level.
             * @param level The verbosity level (basic, detailed).
             */
            void SetVerboseLevel(verbose_level level);

            /**
             * @brief Sets the TCP port for the client.
             * @param port The TCP port to set.
             * @return True if the port was set successfully, false otherwise.
             */
            bool SetTcpPort(uint16_t port);

            /**
             * @brief Sets the UDP port for the client.
             * @param port The UDP port to set.
             * @return True if the port was set successfully, false otherwise.
             */
            bool SetUdpPort(uint16_t port);

            /**
             * @brief Sets the local UDP port for the client.
             * @param port The local UDP port to set.
             * @return True if the local port was set successfully, false otherwise.
             */
            bool SetLocalUdpPort(uint16_t port);

            /**
             * @brief Sets the host for the client.
             * @param host The host address (IP or hostname) to set.
             * @return True if the host was set successfully, false otherwise.
             */
            bool SetHost(const std::string& host);

            /**
             * @brief Checks if the client is connected.
             * @return True if the client is connected, false otherwise.
             */
            [[nodiscard]] bool IsConnected() const;

        private:
            /**
             * @var verbose_
             * @brief Flag indicating whether verbose logging is enabled.
             */
            bool verbose_ = false;

            /**
             * @var level_
             * @brief Verbosity level for logging.
             */
            verbose_level level_ = basic;

            /**
             * @var client_id_
             * @brief Client ID assigned to the client instance.
             */
            uint16_t client_id_ = 0;

            /**
             * @var tcp_client_
             * @brief The TCP client instance.
             */
            TCPClient tcp_client_;

            /**
             * @var udp_client_
             * @brief The UDP client instance.
             */
            UDPClient udp_client_;

            /**
             * @var mutex_
             * @brief Mutex for synchronizing access to shared resources.
             */
            std::mutex mutex_;

            /**
             * @var packets_
             * @brief Vector to store received packets.
             */
            std::vector<packet_t> packets_;

            /**
             * @brief Listens for incoming data over TCP.
             */
            void TCPListenRoutine();

            /**
             * @brief Listens for incoming data over UDP.
             */
            void UDPListenRoutine();

            /**
             * @brief Sends a UDP packet.
             * @param packet The packet to send.
             * @return True if the packet was sent successfully, false otherwise.
             */
            bool UDPSend(packet_t* packet);

            /**
             * @brief Sends a TCP packet.
             * @param packet The packet to send.
             * @return True if the packet was sent successfully, false otherwise.
             */
            bool TCPSend(packet_t* packet);

            /**
             * @brief Checks if a port is open.
             * @param port The port to check.
             * @return True if the port is open, false otherwise.
             */
            [[nodiscard]] bool IsPortOpen(uint16_t port) const;

            /**
             * @brief Checks if a port is valid.
             * @param port The port to validate.
             * @return True if the port is valid, false otherwise.
             */
            [[nodiscard]] bool IsPortValid(uint16_t port) const;

            /**
             * @brief Checks if a host is valid.
             * @param host The host to validate.
             * @return True if the host is valid, false otherwise.
             */
            [[nodiscard]] bool IsHostValid(const std::string& host) const;
    };
}

#endif /* CLIENT_HPP */
