#include <cstdlib>
#include "Abn.hpp"

using namespace ABN;

int main()
{
    Server server(4242, 4243, true);
    Client client(4242, 4243, 5050, "127.0.0.1", true, debug);

    if (!server.Open()) {
        std::cerr << "Error opening server." << std::endl;
        return 1;
    }

    if (!client.Connect()) {
        std::cerr << "Error connecting client." << std::endl;
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

    if (!server.Send(&t, sizeof(test_t), 10, false))
        std::cerr << "Error sending message." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    client.GetPackets(packets_for_server);
    if (packets_for_server.size() < 1) {
        std::cerr << "No packets received." << std::endl;
        return 1;
    }
    std::cout << packets_for_server.size() << std::endl;
    if (!client.Disconnect()) {
        std::cerr << "Error disconnecting client." << std::endl;
        return 1;
    }
    if (!server.Close()) {
        std::cerr << "Error closing server." << std::endl;
        return 1;
    }
}