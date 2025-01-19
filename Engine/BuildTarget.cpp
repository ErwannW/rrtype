//
// Created by Nail on 20/11/2024.
//

#include "BuildTarget.hpp"

namespace MGE::BuildTarget
{
    std::string ToString(const OperatingSystem operatingSystem)
    {
        return std::string(PlatformName(operatingSystem));
    }

    std::string ToString(const BuildType buildType)
    {
        switch (buildType) {
            case BuildType::Client:
                return "Client";
            case BuildType::Server:
                return "Server";
        }
        return "Unknown";
    }

    std::string ToString(const BuildConfiguration buildConfiguration)
    {
        switch (buildConfiguration)
        {
            case BuildConfiguration::Debug:
                return "Debug";
            case BuildConfiguration::Release:
                return "Release";
        }
        return "Unknown";
    }
}
