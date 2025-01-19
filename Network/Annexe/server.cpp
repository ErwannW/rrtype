//
// Created by ewicart on 05.12.24.
//

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

    int pp = 0;

    if (pp == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        server.Send(&t, sizeof(test_t), 10, false);
    }

    if (pp == 1) {
        while (true) {
            server.GetPackets(packets_for_server);
            if (!packets_for_server.empty()) {
                const packet_t i = packets_for_server.back();
                std::cout << "Client ID: " << i.header_.client_id_ << std::endl;
                std::cout << "Command code: " << i.header_.command_code_ << std::endl;
                std::cout << "Body size: " << i.header_.body_size_ << std::endl;
                if (i.header_.command_code_ == 10) {
                    test_t tt;
                    std::memcpy(&tt, i.body_.data(), sizeof(test_t));

                    std::cout << "magic_number: " << tt.magic_number << std::endl;
                    std::cout << "magic_roll: " << tt.magic_roll << std::endl;
                    std::cout << "magic_string: " << tt.magic_string << std::endl;
                }
                packets_for_server.pop_back();
            }
        }
        return 0;
    }
}
