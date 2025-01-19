//
// Created by ewicart on 05.12.24.
//

#include <cstdlib>
#include "Client.hpp"

using namespace ABN;

int main(int ac, char **av)
{
    Client client(4242, 4243, 5050, "127.0.0.1", true);

    if (!client.Connect()) {
        return 1;
    }

    std::vector<packet_t> packets_for_client;

    std::srand(std::time(nullptr));
    struct test_t {
        uint32_t magic_number = 42;
        int magic_roll = std::rand() % 100;
        const char magic_string[25] = "Hello world from client!";
    };
    const test_t t;

    int pp = 1;

    if (pp == 0)
        client.Send(&t, sizeof(test_t), 10, false);

    if (pp == 1) {
        while (true) {
            client.GetPackets(packets_for_client);
            if (!packets_for_client.empty()) {
                const packet_t i = packets_for_client.back();
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
                packets_for_client.pop_back();
            }
        }
        return 0;
    }
}
