//
// Created by Nail on 27/09/2024.
// Provides abstractions for platform-dependent library loading functionality.
//

#ifndef LIBLOADINGAPI_HPP
#define LIBLOADINGAPI_HPP

#if WINDOWS
#include <windows.h>
#include <libloaderapi.h>
#else
#include <dlfcn.h>
#endif
#include <string>

namespace LibLoadingAPI {
    using HandleType =
#if WINDOWS
    HMODULE;
#else
        void*;
#endif

    using SymbolAddress =
#if WINDOWS
    FARPROC;
#else
    void*;
#endif

    HandleType LoadLib(std::string const &path);
    void UnloadLib(HandleType handle);
    SymbolAddress GetSymbolAddress(HandleType handle, std::string const &symbol);
}

#endif //LIBLOADINGAPI_HPP
