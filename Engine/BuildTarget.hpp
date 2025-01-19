//
// Created by Nail on 27/09/2024.
//

#ifndef BUILDTARGET_HPP
#define BUILDTARGET_HPP

#include <cstdint>
#include <string>

namespace MGE::BuildTarget
{
    enum class BuildConfiguration : std::uint8_t {
        Debug,
        Release
    };

    enum class OperatingSystem : std::uint8_t {
        Windows,
        Linux,
        MacOSX
    };

    enum class BuildType : std::uint8_t {
        Client,
        Server
    };

#if WINDOWS
    static constexpr auto CurrentOperatingSystem = OperatingSystem::Windows;
#elif LINUX
    static constexpr auto CurrentOperatingSystem = OperatingSystem::Linux;
#elif MACOSX
    static constexpr auto CurrentOperatingSystem = OperatingSystem::MacOSX;
#else
    static_assert(false, "You are using an unsuported target platform.");
#endif

#if DEBUG_BUILD
    static constexpr auto CurrentBuildConfiguration = BuildConfiguration::Debug;
#elif RELEASE_BUILD
    static constexpr auto CurrentBuildConfiguration = BuildConfiguration::Release;
#else
    static_assert(false, "Invalid configuration, check you build settings.");
#endif

#ifdef CLIENT_BUILD
    static constexpr auto CurrentBuildType = BuildType::Client;
#elifdef SERVER_BUILD
    static constexpr auto CurrentBuildType = BuildType::Server;
#else
    static_assert(false, "Invalid configuration, check you build settings.");
#endif

    constexpr std::string_view PlatformName(OperatingSystem operatingSystem = CurrentOperatingSystem)
    {
        switch (CurrentOperatingSystem) {
            case OperatingSystem::Windows:
                return "Windows";
            case OperatingSystem::Linux:
                return "Linux";
            case OperatingSystem::MacOSX:
                return "MacOSX";
        }
        return "Unknown";
    }

    consteval bool IsServerBuild() noexcept
    {
        return CurrentBuildType == BuildType::Server;
    }

    consteval bool IsClientBuild()
    {
        return CurrentBuildType == BuildType::Client;
    }

    std::string ToString(BuildType buildType);
    std::string ToString(BuildConfiguration buildConfiguration);
    std::string ToString(OperatingSystem operatingSystem);
}


#endif //BUILDTARGET_HPP
