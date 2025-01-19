//
// Created by Nail on 27/09/2024.
//


#include "LibLoadingAPI.hpp"

namespace LibLoadingAPI
{
    HandleType LoadLib(std::string const &path)
    {
#if WINDOWS
        return LoadLibraryA(path.c_str());
#else
        return dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif
    }

    void UnloadLib(HandleType handle)
    {
#if WINDOWS
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
    }

    SymbolAddress GetSymbolAddress(HandleType handle, std::string const &symbol)
    {
#if WINDOWS
        return GetProcAddress(handle, symbol.c_str());
#else
        return dlsym(handle, symbol.c_str());
#endif
    }
}
