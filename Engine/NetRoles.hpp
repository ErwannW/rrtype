//
// Created by Nail on 19/11/2024.
//

#ifndef NETROLES_HPP
#define NETROLES_HPP

#include <cstdint>
#include <string>

namespace MGE
{
    /**
     * Define the possible network roles of the running process.
     */
    enum class NetRole : std::uint8_t
    {
        None          = 0b00000000, /**> This is not a valid role and should only be used as a default/error value.*/
        Client        = 0b00000001, /**> This process is a client that will connect to a remote server. */
        Host          = 0b00000010, /**> This process is the server the clients will connect to. It is the authority of the game. */
        HostingClient = Client | Host, /**> This process is both a client and the host. Useful for peer-to-peer networking.*/
    };

    const std::string& ToString(NetRole role);
}

#endif // NETROLES_HPP
