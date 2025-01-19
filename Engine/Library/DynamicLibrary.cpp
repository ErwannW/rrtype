//
// Created by Nail on 27/09/2024.
//

#include "DynamicLibrary.hpp"

#include "BuildTarget.hpp"

namespace MGE
{
    DynamicLib::DynamicLib(char const *&&path) : _path(path) { }

    DynamicLib::DynamicLib(std::filesystem::path const &path) : _path(path) { }

    DynamicLib::DynamicLib(std::filesystem::path &&path) : _path(std::move(path)) { }

    DynamicLib::DynamicLib(DynamicLib &&other) noexcept : _path(std::move(other._path)), _handle(other._handle)
    {
        other._handle = nullptr;
    }

    DynamicLib::~DynamicLib()
    {
        UnLoad();
    }

    void DynamicLib::Load()
    {
        _handle = LibLoadingAPI::LoadLib(_path.string());
    }

    void DynamicLib::UnLoad()
    {
        if (!IsLoaded())
            return;

        LibLoadingAPI::UnloadLib(_handle);
        _handle = nullptr;
    }

    bool DynamicLib::IsLoaded() const noexcept
    {
        return _handle != nullptr;
    }

    std::filesystem::path const &DynamicLib::GetPath() const noexcept
    {
        return _path;
    }

    void *DynamicLib::operator[](std::string const &name) const
    {
        return GetAddress<void>(name);
    }

    bool DynamicLib::HasSymbol(const std::string &name) const
    {
        return GetAddress(name) != nullptr;
    }

    LibLoadingAPI::HandleType DynamicLib::GetHandle() const noexcept
    {
        return _handle;
    }

    std::string const &DynamicLib::SystemExtension()
    {
        if constexpr (BuildTarget::CurrentOperatingSystem == BuildTarget::OperatingSystem::Windows) {
            static std::string const exp = ".dll";
            return exp;
        } else {
            static std::string const exp = ".so";
            return exp;
        }
    }
} // MGE