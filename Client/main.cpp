/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** main.cpp
*/

#include "Client.hpp"

static int error_handling(const int ac, char **av)
{
    if (ac == 1 && std::string(av[0]) == "-help") {
        std::cout << "./Client host port\n\thost\tlitteral or numerical hostname\n\tport\tport number" << std::endl;
        exit(0);
    } else if (ac == 2) {
        boost::system::error_code ec;
        boost::asio::ip::make_address(av[0], ec);

        if (std::string(av[0]) != "localhost" && ec)
            return (FAIL);

        try {
            const int port(std::stoi(av[1]));
            if (port < 1024 || port > 65535)
                return (FAIL);
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            return (FAIL);
        }

        return (SUCCESS);
    }
    return (FAIL);
}

int main(int ac, char **av)
{
    if (error_handling(ac - 1, av + 1) == FAIL)
        return (FAIL);

    std::string s = "";
    auto a = std::make_shared<std::string>(s);

    try {
        UDP::Client<std::string> client(static_cast<uint16_t>(std::stoi(av[2])), av[1], true, a);
        client.listenToServer();
        while (true) {}
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

}
