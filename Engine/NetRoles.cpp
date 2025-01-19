//
// Created by nailperreau on 20/11/24.
//

#include "NetRoles.hpp"

#include <string>

const std::string& MGE::ToString(const NetRole role)
{
    static const std::string None          = "None";
    static const std::string Client        = "Client";
    static const std::string Host          = "Host";
    static const std::string HostingClient = "Hosting Client";
    static const std::string Unknown       = "Unknown";

    switch (role) {
        case NetRole::None:
            return None;
        case NetRole::Client:
            return Client;
        case NetRole::Host:
            return Host;
        case NetRole::HostingClient:
            return HostingClient;
    }
    return Unknown;
}
