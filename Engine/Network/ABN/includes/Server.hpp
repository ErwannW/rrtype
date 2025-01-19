/*
** EPITECH PROJECT, 2024
** Server.cpp
** File description:
** Definition for Server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Misc.hpp"

namespace ABN
{
    /**
     * @struct TCPServer
     * @brief Structure to represent a TCP server.
     *
     * This structure encapsulates all necessary data and functions
     * for setting up a TCP server using ASIO. It manages the connection
     * and communication via TCP protocol.
     */
    struct TCPServer
    {
        /**
         * @var opened_
         * @brief Flag indicating if the server is open or not.
         */
        bool opened_ = false;

        /**
         * @var port_
         * @brief Port the server will listen on.
         */
        uint16_t port_ = 8080;

        /**
         * @var io_context_
         * @brief The ASIO I/O context for handling asynchronous events.
         */
        asio::io_context io_context_ = asio::io_context();

        /**
         * @var server_endpoint_
         * @brief The endpoint (IP address + port) where the server will listen.
         */
        asio::ip::tcp::endpoint server_endpoint_;

        /**
         * @var acceptor_
         * @brief The TCP acceptor used for accepting incoming connections.
         */
        asio::ip::tcp::acceptor acceptor_;

        /**
         * @var acceptor_thread_
         * @brief The thread dedicated to accepting incoming connections.
         */
        std::thread acceptor_thread_;

        /**
         * @var listen_threads_
         * @brief Vector of threads for listening to connected clients.
         */
        std::vector<std::unique_ptr<std::thread>> listen_threads_;

        /**
         * @brief Constructor to initialize the TCP server with optional verbosity.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The level of verbosity (basic, detailed).
         */
        explicit TCPServer(const bool verbose = false, const verbose_level level = basic)
            : server_endpoint_(asio::ip::tcp::v4(), port_), acceptor_(io_context_, server_endpoint_)
        {
            if (verbose)
                ABN_DEBUG(SERVER_DEBUG, "[TCPSERVER]", "Created")
        };

        /**
         * @brief Constructor to initialize the TCP server with a specific port and optional verbosity.
         * @param port The port the server will listen on.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The level of verbosity (basic, detailed).
         */
        explicit TCPServer(const uint16_t port, const bool verbose = false, const verbose_level level = basic)
            : port_(port), server_endpoint_(asio::ip::tcp::v4(), port_), acceptor_(io_context_, server_endpoint_)
        {
            if (verbose)
                ABN_DEBUG(SERVER_DEBUG, "[TCPSERVER]", "Created")
        };
    };

    /**
     * @struct UDPServer
     * @brief Structure to represent a UDP server.
     *
     * This structure encapsulates all necessary data and functions
     * for setting up a UDP server using ASIO. It manages the communication
     * via UDP protocol.
     */
    struct UDPServer
    {
        /**
         * @var opened_
         * @brief Flag indicating if the server is open or not.
         */
        bool opened_ = false;

        /**
         * @var port_
         * @brief Port the server will listen on.
         */
        uint16_t port_ = 8081;

        /**
         * @var io_context_
         * @brief The ASIO I/O context for handling asynchronous events.
         */
        asio::io_context io_context_ = asio::io_context();

        /**
         * @var local_endpoint_
         * @brief The endpoint (IP address + port) where the server will listen for UDP packets.
         */
        asio::ip::udp::endpoint local_endpoint_;

        /**
         * @var socket_
         * @brief The UDP socket used for receiving and sending packets.
         */
        asio::ip::udp::socket socket_;

        /**
         * @var listen_thread_
         * @brief The thread dedicated to listening for incoming UDP packets.
         */
        std::thread listen_thread_;

        /**
         * @brief Constructor to initialize the UDP server with optional verbosity.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The level of verbosity (basic, detailed).
         */
        explicit UDPServer(const bool verbose = false, const verbose_level level = basic)
            : local_endpoint_(asio::ip::udp::v4(), port_), socket_(io_context_, local_endpoint_)
        {
            if (verbose)
                ABN_DEBUG(SERVER_DEBUG, "[UDPSERVER]", "Created")
        };

        /**
         * @brief Constructor to initialize the UDP server with a specific port and optional verbosity.
         * @param port The port the server will listen on.
         * @param verbose A boolean to enable or disable verbose logging.
         * @param level The level of verbosity (basic, detailed).
         */
        explicit UDPServer(const uint16_t port, const bool verbose = false, const verbose_level level = basic)
            : port_(port), local_endpoint_(asio::ip::udp::v4(), port_), socket_(io_context_, local_endpoint_)
        {
            if (verbose)
                ABN_DEBUG(SERVER_DEBUG, "[UDPSERVER]", "Created")
        };
    };

    /**
     * @struct ClientStruct
     * @brief Structure to represent a connected client.
     *
     * This structure stores information about a client, such as its
     * IP address, UDP port, and associated TCP socket.
     */
    struct ClientStruct
    {
        /**
         * @var client_id_
         * @brief Unique identifier for the client.
         */
        uint16_t client_id_ = 0;

        /**
         * @var ip_address_
         * @brief IP address of the client.
         */
        std::string ip_address_;

        /**
         * @var udp_port_
         * @brief The UDP port the client is listening on.
         */
        uint16_t udp_port_ = 0;

        /**
         * @var client_tcp_socket_
         * @brief Shared pointer to the client's TCP socket.
         */
        std::shared_ptr<asio::ip::tcp::socket> client_tcp_socket_ = nullptr;
    };

    /**
     * @class Server
     * @brief Class representing the server that manages both TCP and UDP communication.
     *
     * This class provides functions to initialize the server, handle incoming connections,
     * send packets over TCP/UDP, and manage clients.
     */
    class Server {
        public:
            /**
             * @brief Default constructor for the server.
             */
            Server() = default;

            /**
             * @brief Constructor to initialize the server with specific TCP and UDP ports.
             * @param tcp_port The port for the TCP server.
             * @param udp_port The port for the UDP server.
             */
            Server(uint16_t tcp_port, uint16_t udp_port);

            /**
             * @brief Constructor to initialize the server with specific ports and verbosity.
             * @param tcp_port The port for the TCP server.
             * @param udp_port The port for the UDP server.
             * @param verbose A boolean to enable or disable verbose logging.
             */
            Server(uint16_t tcp_port, uint16_t udp_port, bool verbose);

            /**
             * @brief Constructor to initialize the server with specific ports, verbosity, and verbose level.
             * @param tcp_port The port for the TCP server.
             * @param udp_port The port for the UDP server.
             * @param verbose A boolean to enable or disable verbose logging.
             * @param level The level of verbosity (basic, detailed).
             */
            Server(uint16_t tcp_port, uint16_t udp_port, bool verbose, verbose_level level);

            /**
             * @brief Destructor for the server, cleaning up resources.
             */
            ~Server();

            /**
             * @brief Opens the server and prepares it to accept connections.
             * @return True if the server opened successfully, false otherwise.
             */
            bool Open();

            /**
             * @brief Closes the server and terminates all active connections.
             * @return True if the server closed successfully, false otherwise.
             */
            bool Close();

            /**
             * @brief Sends a packet to the server's clients.
             * @param data The data to be sent.
             * @param body_size The size of the data.
             * @param command_code The command code for the message.
             * @param reliable Indicates whether the packet should be sent using TCP (reliable) or UDP (unreliable).
             * @return True if the packet was sent successfully, false otherwise.
             *
             * The reliable parameter determines the transport protocol:
             * if true, TCP is used; if false, UDP is used.
             */
            bool Send(const void* data, uint16_t body_size, uint16_t command_code, bool reliable);

            /**
             * @brief Sends a packet to a specific client.
             * @param data The data to be sent.
             * @param body_size The size of the data.
             * @param command_code The command code for the message.
             * @param reliable Indicates whether the packet should be sent using TCP (reliable) or UDP (unreliable).
             * @param client_id The ID of the client to send the packet to.
             * @return True if the packet was sent successfully, false otherwise.
             *
             * The reliable parameter determines the transport protocol:
             * if true, TCP is used; if false, UDP is used.
             */
            bool Send(const void* data, uint16_t body_size, uint16_t command_code, bool reliable, uint16_t client_id);

            /**
             * @brief Retrieves all the received packets.
             * @param packets A vector to store the retrieved packets.
             */
            void GetPackets(std::vector<packet_t>& packets);

            /**
             * @brief Sets the TCP port for the server.
             * @param port The port number to set for the TCP server.
             * @return True if the port was successfully set, false otherwise.
             */
            bool SetTcpPort(uint16_t port);

            /**
             * @brief Sets the UDP port for the server.
             * @param port The port number to set for the UDP server.
             * @return True if the port was successfully set, false otherwise.
             */
            bool SetUdpPort(uint16_t port);

            /**
             * @brief Sets the verbosity of the server logging.
             * @param verbose A boolean to enable or disable verbose logging.
             */
            void SetVerbose(bool verbose);

            /**
             * @brief Sets the verbosity level for the server.
             * @param level The verbosity level (basic, detailed).
             */
            void SetVerboseLevel(verbose_level level);

            /**
             * @brief Checks whether the server is currently open.
             * @return True if the server is open, false otherwise.
             */
            [[nodiscard]] bool IsOpen() const;

        private:
            /**
             * @var verbose_
             * @brief Flag indicating whether verbose logging is enabled.
             */
            bool verbose_ = false;

            /**
             * @var level_
             * @brief The verbosity level (basic, detailed).
             */
            verbose_level level_ = basic;

            /**
             * @var client_id_
             * @brief The client ID that is assigned to each new client.
             */
            uint16_t client_id_ = 1;

            /**
             * @var clients_
             * @brief A list of all connected clients.
             */
            std::vector<ClientStruct> clients_;

            /**
             * @var mutex_
             * @brief A mutex for synchronizing access to shared resources.
             */
            std::mutex mutex_;

            /**
             * @var packets_
             * @brief A vector of all received packets.
             */
            std::vector<packet_t> packets_;

            /**
             * @var tcp_server_
             * @brief The TCP server instance.
             */
            TCPServer tcp_server_;

            /**
             * @var udp_server_
             * @brief The UDP server instance.
             */
            UDPServer udp_server_;

            /**
             * @brief Checks if a given port is available for use.
             * @param port The port number to check.
             * @return True if the port is free, false otherwise.
             */
            [[nodiscard]] bool IsPortFree(uint16_t port) const;

            /**
             * @brief Checks if a given client ID is known to the server.
             * @param client_id The client ID to check.
             * @return True if the client ID is known, false otherwise.
             */
            [[nodiscard]] bool IsKnownClientID(uint16_t client_id) const;

            /**
             * @brief Sends a UDP packet to a client.
             * @param packet The packet to send.
             * @param ip_addr The IP address of the client.
             * @param port The port to send the packet to.
             * @return True if the packet was sent successfully, false otherwise.
             */
            bool UDPSend(packet_t* packet, const std::string& ip_addr, uint16_t port);

            /**
             * @brief Sends a TCP packet to a client.
             * @param packet The packet to send.
             * @param socket The TCP socket to use for sending the packet.
             * @return True if the packet was sent successfully, false otherwise.
             */
            bool TCPSend(packet_t* packet, const std::shared_ptr<asio::ip::tcp::socket>& socket);

            /**
             * @brief Handles incoming TCP connections.
             */
            void AcceptorRoutine();

            /**
             * @brief Processes commands from received packets.
             * @param packet The packet containing the command.
             * @return True if the command was handled successfully, false otherwise.
             */
            bool ProtectedCommandHandling(const packet_t& packet);

            /**
             * @brief Listens for incoming data on the TCP connection for a specific client.
             * @param client The client to listen for data from.
             */
            void TCPListenRoutine(const ClientStruct& client);

            /**
             * @brief Listens for incoming UDP packets.
             */
            void UDPListenRoutine();
    };
}

#endif /* SERVER_HPP */
