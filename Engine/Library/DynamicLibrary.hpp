//
// Created by Nail on 27/09/2024.
//

#ifndef DYNAMICLIBRARY_HPP
#define DYNAMICLIBRARY_HPP

#include <string>
#include <filesystem>
#include "LibLoadingAPI.hpp"

namespace MGE
{
    class DynamicLib
    {
        public:
            DynamicLib() = delete;
            explicit DynamicLib(char const *&&path);
            explicit DynamicLib(std::filesystem::path const &path); // NOLINT(*-explicit-constructor)
            explicit DynamicLib(std::filesystem::path &&path); // NOLINT(*-explicit-constructor)

            DynamicLib(DynamicLib const &) = delete;
            DynamicLib(DynamicLib &&other) noexcept;

            ~DynamicLib();

            void Load();
            void UnLoad();
            [[nodiscard]] bool IsLoaded() const noexcept;

            /**
             * @returns The path to this library.
             */
            [[nodiscard]] const std::filesystem::path &GetPath() const noexcept;

            /**
             * @brief Calls the functionName associated with this symbol.
             * @returns The value returned by this function call, if non-void.
             */
            template<typename TFunction, typename ...TArgs>
            std::invoke_result_t<TFunction, TArgs...> Call(std::string const &functionName, TArgs&&... args) const;

            /**
             * @returns The address to this symbol.
             */
            [[nodiscard]] void *operator[](std::string const &name) const;

            /**
             * @returns The address of this symbol.
             */
            template<typename T = void>
            [[nodiscard]] T *GetAddress(std::string const &name) const;

            /**
             * Non-const variant of GetAddress that automatically loads the library if it isn't already loaded.
             * @returns The address of this symbol.
             */
            template<typename T = void>
            [[nodiscard]] T* GetAddress(std::string const &name);

            [[nodiscard]] bool HasSymbol(const std::string& name) const;

            /**
             * @brief Gets the address of the given symbol and stores it in OutAddress.
             * @note OutAddress is left unchanged if failed.
             * @returns True if success, false otherwise.
             */
            template<typename T>
            bool TryGetAddress(std::string const &name, T* &outAddress) const noexcept;

            /**
             * @returns A reference to the value contained in the address of this symbol.
             */
            template<typename T>
            [[nodiscard]] T &GetValue(std::string const &name) const;

            /**
             * @returns The handle of this library. Take care of it!
             */
            [[nodiscard]] LibLoadingAPI::HandleType GetHandle() const noexcept;

            DynamicLib &operator=(DynamicLib const &other) = delete;

            static std::string const &SystemExtension();

        protected:
            std::filesystem::path _path;
            LibLoadingAPI::HandleType _handle{};
    };

    template<typename T>
    bool DynamicLib::TryGetAddress(std::string const &name, T *&outAddress) const noexcept
    {
        void* address = GetAddress<void>(name);

        if (address == nullptr)
            return false;

        outAddress = static_cast<T*>(address);
        return true;
    }

    template<typename TValue>
    TValue &DynamicLib::GetValue(std::string const &name) const
    {
        TValue* const ptr = GetAddress<TValue>(name);

        if (ptr == nullptr)
            throw std::runtime_error("Could not find symbol.");
        return *ptr;
    }

    template<typename TSymbol>
    TSymbol* DynamicLib::GetAddress(std::string const &name) const
    {
        if (!IsLoaded())
            throw std::logic_error("Library is not loaded.");

        return reinterpret_cast<TSymbol*>(LibLoadingAPI::GetSymbolAddress(_handle, name));
    }

    template<typename TSymbol>
    TSymbol* DynamicLib::GetAddress(std::string const &name)
    {
        if (!IsLoaded())
            Load();

        return GetAddress<TSymbol>(name);
    }

    template<typename TFunction, typename ...TArgs>
    std::invoke_result_t<TFunction, TArgs...> DynamicLib::Call(std::string const &functionName, TArgs&&... args) const
    {
        TFunction* const func = GetAddress<TFunction>(functionName);

        if (func == nullptr)
            throw std::runtime_error("Symbol not found.");

        return func(std::forward<TArgs>(args)...);
    }
} // My

#endif //DYNAMICLIBRARY_HPP
