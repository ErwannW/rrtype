# Asio Based Network (ABN) Server-Client technical documentation

This documentation is a step-by-step tutorial to learn how to use ABN lib

## Dependency

- C++ compiler (any kind)
- [CMake](https://cmake.org/download/)
- [Asio lib (1.30.2)](https://sourceforge.net/projects/asio/files/asio/1.30.2%20%28Stable%29/)

## Structure

The ABN lib consists of two main parts:

1. **Server**
2. **Client**

Both server and client will communicate using a
packed bytes message describe in RFC 1ABN protocol

They will stock in their object, a vector, where will be
contained each packet that has been received, you can ask for
those packets at any time in the program.

### Packet

```cpp
namespace ABN {
    #pragma pack(push, 1)
    struct header_t
    {
        uint16_t command_code_;
        uint16_t client_id_;
        uint16_t body_size_;
    };

    struct packet_t
    {
        header_t header_;
        std::array<uint8_t, IPV4_MTU_HEADER_LESS> body_;

    };
    #pragma pack(pop)
}
```

This is the way that we stock a single packet, we handle a
```std::vector<packet_t>``` in both server and client.

### Consideration

Knowing that both server and client are communicating using TCP and UDP,
when you have to send a message, the last parameters will always be
if the message has to be transmitted reliably or not, so if it's true
then TCP, otherwise UDP:

```cpp
server.Send(..., true); -> Sending TCP
server.Send(..., false); -> Sending UDP
```

```cpp
client.Send(..., true); -> Sending TCP
client.Send(..., false); -> Sending UDP
```

### Server

The server listens for incoming connections from clients and sends structured data to the client once connected. It processes any incoming packets and logs the content of the packets.

#### Example of use for the Server:

There are many ways to construct a server, we have to define where he's going
to listen for client, tcp and udp. Verbose and Verbose level are for debug
purpose only, you should not use them in production.

```cpp
Server() = default;
Server(uint16_t tcp_port, uint16_t udp_port);
Server(uint16_t tcp_port, uint16_t udp_port, bool verbose);
Server(uint16_t tcp_port, uint16_t udp_port, bool verbose, verbose_level level);
```

Default settings are 8080 tcp port, 8081 udp port, non-verbal.

Here you can find a quick example that how could you use the server.

```cpp
#include <cstdlib>
#include "Server.hpp"

using namespace ABN;

int main()
{
    Server server(4242, 4243, true);

    if (!server.Open()) {
        return 1;
    }

    std::vector<packet_t> packets_for_server;

    std::srand(std::time(nullptr));
    struct test_t {
        uint32_t magic_number = 42;
        int magic_roll = std::rand() % 100;
        const char magic_string[25] = "Hello world from server!";
    };
    const test_t t;

    server.Send(&t, sizeof(test_t), 10, false);

    while (true) {
        server.GetPackets(packets_for_server);
        if (!packets_for_server.empty()) {
            for (const packet_t& i : packets_for_server) {
                std::cout << "Client ID: " << i.header_.client_id_ << std::endl;
                std::cout << "Command code: " << i.header_.command_code_ << std::endl;
                std::cout << "Body size: " << i.header_.body_size_ << std::endl;

                test_t tt;
                std::memcpy(&tt, i.body_.data(), sizeof(test_t));

                std::cout << "magic_number: " << tt.magic_number << std::endl;
                std::cout << "magic_roll: " << tt.magic_roll << std::endl;
                std::cout << "magic_string: " << tt.magic_string << std::endl;
            }
            break;
        }
    }
    return 0;
}
```

### Client

The client listens for incoming connections from clients and sends structured data he's connected to the server. It processes any incoming packets and logs the content of the packets.

#### Example of use for the Client:

There are many ways to construct a client, we have to define where he's going
to connect to the server, tcp and udp, and at which port he will listen udp message.
Verbose and Verbose level are for debug purpose only, you should not use them in production.

```cpp
Client() = default;
Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port);
Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, bool verbose);
Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, bool verbose, verbose_level level);
Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, const std::string& host);
Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, const std::string& host, bool verbose);
Client(uint16_t tcp_port, uint16_t udp_port, uint16_t local_udp_port, const std::string& host, bool verbose, verbose_level level);
```

Default settings are 8080 tcp port, 8081 udp port, 12345 local_udp port, localhost, non-verbal.

Here you can find a quick example that how could you use the client.

```cpp
#include <cstdlib>
#include "Client.hpp"

using namespace ABN;

int main()
{
    Client client(4242, 4243, 4254, "127.0.0.1", true);

    if (!client.Connect()) {
        return 1;
    }

    std::vector<packet_t> packets_for_client;

    std::srand(std::time(nullptr));
    struct test_t {
        uint32_t magic_number = 42;
        int magic_roll = std::rand() % 100;
        const char magic_string[25] = "Hello world from server!";
    };
    const test_t t;

    client.Send(&t, sizeof(test_t), 10, true);

    while (true) {
        client.GetPackets(packets_for_client);
        if (!packets_for_client.empty()) {
            for (const packet_t& i : packets_for_client) {
                std::cout << "Client ID: " << i.header_.client_id_ << std::endl;
                std::cout << "Command code: " << i.header_.command_code_ << std::endl;
                std::cout << "Body size: " << i.header_.body_size_ << std::endl;

                test_t tt;
                std::memcpy(&tt, i.body_.data(), sizeof(test_t));

                std::cout << "magic_number: " << tt.magic_number << std::endl;
                std::cout << "magic_roll: " << tt.magic_roll << std::endl;
                std::cout << "magic_string: " << tt.magic_string << std::endl;
            }
            break;
        }
    }
    return (0);
}
```

You can have all technical kind information in the doxygen documentation.
