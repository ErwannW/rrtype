//
// Created by np on 12/2/24.
//

#ifndef RTYPE_CLASS_HPP
#define RTYPE_CLASS_HPP

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <type_traits>
#include "Property.hpp"
#include "Hashing/Hash.hpp"
#include "Asserts.hpp"
#include "Rpc.hpp"

#define TypeUsing _Type
#define RegisterProperty(prop, ...) AddProperty<TypeUsing, std::decay_t<decltype(prop)>>(#prop, &TypeUsing::prop __VA_OPT__(,)__VA_ARGS__)
#define RegisterRPC(prop, target, ...) AddRPC<TypeUsing>(#prop, target, &TypeUsing::prop __VA_OPT__(,)__VA_ARGS__)
#define REPLICATED MGE::Property::Attributes::Replicated

#define DECLARE_STATIC_CLASS(c) static inline MGE::Class Class = MGE::Class(#c)
#define DECLARE_SETUP_REFLECTION_METHOD static void SetupReflection()
#define STATIC_GET_CLASS(c) static const MGE::Class &GetClass() { static bool initialized = false; if (!initialized) { initialized = true; Class.size = sizeof(c); SetupReflection(); } return Class; }

#define REFLECT_CLASS (c) private: DECLARE_STATIC_CLASS(c); DECLARE_SETUP_REFLECTION_METHOD; public: using TypeUsing = c; STATIC_GET_CLASS(c); private:
#define REFLECT_STRUCT(c) private: DECLARE_STATIC_CLASS(c); DECLARE_SETUP_REFLECTION_METHOD; public: using TypeUsing = c; STATIC_GET_CLASS(c);

namespace MGE
{
    #define Struct MGE::Class

    /**
     * Represents a class for the reflection system.
     */
    class Class
    {
        public:
            explicit constexpr Class(std::string name);

            Class(const Class& other) = delete;
            Class &operator=(const Class& other) = delete;

            /**
             * @return The name of this class.
             */
            [[nodiscard]] constexpr const std::string& GetName() const noexcept;

            /**
             * Register a property of this class.
             * @tparam TTarget The target type of the property (this class).
             * @tparam TProperty The type of the property.
             * @tparam TArgs An additional arguments that will be forwarded to the Property class constructor.
             * @param propertyName The name of the property.
             * @param member A member pointer to the property that is added.
             * @param args The list of args that will be forwarded to the Property class constructor.
             */
            template<typename TTarget, typename TProperty, typename... TArgs>
            constexpr void AddProperty(const std::string& propertyName, TProperty TTarget::*member, TArgs&&... args)
            {
                properties.emplace_back(*this, propertyName, member, std::forward<TArgs>(args)...);
            }

            /**
             * Register a RPC on this class.
             * @tparam TTarget The type of the class. (this class);
             * @tparam TParam The type fo the param of this RPC.
             * @tparam TArgs A list of args that will be forwarded to the RPC class constructor.
             * @param rpcName The name of this RPC.
             * @param target The target of this RPC.
             * @param member A member pointer to this rpc.
             * @param args A list of args that will be forwarded to the RPC class constructor
             */
            template<typename TTarget, typename TParam, typename... TArgs>
            constexpr void AddRPC(const std::string& rpcName, RPC::Target target, RPCMethod<TTarget, TParam> member, TArgs&&... args)
            {
                rpcs.emplace_back(*this, rpcName, target, member, std::forward<TArgs>(args)...);
            }

            /**
             * Checks if the given property exists in this class.
             * @param propertyName The name of the property to look for.
             * @return true if the property exists and is visible by the reflection system, false otherwise.
             */
            [[nodiscard]] constexpr bool HasProperty(std::string_view propertyName) const;
            /**
             * Checks if the given property exists in this class.
             * @param propertyHash The hash of the property to look for.
             * @return true if the property exists and is visible by the reflection system, false otherwise.
             */
            [[nodiscard]] constexpr bool HasProperty(const Hash& propertyHash) const;

            /**
             * Get a property.
             * @param propertyName The name of the property to look for.
             */
            [[nodiscard]] constexpr const Property& GetProperty(std::string_view propertyName) const;
            /**
             * Get a property.
             * @param propertyHash The hash of the property to look for.
             */
            [[nodiscard]] constexpr const Property& GetProperty(const Hash& propertyHash) const;

            /**
             * Tries to get a property.
             * @param propertyName The hash of the property to look for.
             * @return A std::optional with the property as its value, or an empty optional if the property was not found.
             */
            [[nodiscard]] constexpr std::optional<std::reference_wrapper<const Property>> TryGetProperty(std::string_view propertyName) const;
            /**
             * Tries to get a property.
             * @param propertyHash The hash of the property to look for.
             * @return A std::optional with the property as its value, or an empty optional if the property was not found.
             */
            [[nodiscard]] constexpr std::optional<std::reference_wrapper<const Property>> TryGetProperty(const MGE::Hash& propertyHash) const;

            /**
             * @return All properties of this class.
             */
            [[nodiscard]] constexpr const std::vector<Property>& GetProperties() const noexcept;

            /**
             * Get an RPC by its name.
             * @param rpcName The name of the RPC.
             */
            [[nodiscard]] constexpr const RPC& GetRPC(const std::string& rpcName) const;
            /**
             * Get an RPC by its name.
             * @param rpcHash The name of the RPC.
             */
            [[nodiscard]] constexpr const RPC& GetRPC(const Hash& rpcHash) const;

            /**
             * @param rpcName The name of the RPC to look for.
             * @return A std::optional with the RPC as its value, or an empty optional if the property was not found.
             */
            [[nodiscard]] constexpr std::optional<std::reference_wrapper<const RPC>> TryGetRPC(const std::string& rpcName) const;

            /**
             * @param rpcHash The hash of the RPC to look for.
             * @return A std::optional with the RPC as its value, or an empty optional if the property was not found.
             */
            [[nodiscard]] constexpr std::optional<std::reference_wrapper<const RPC>> TryGetRPC(const MGE::Hash& rpcHash) const;

            /**
             * @return All RPCs of this class.
             */
            [[nodiscard]] constexpr const std::vector<RPC>& GetRPCs() const noexcept;

            /**
             * @return true if the class has any property marked as replicated, false otherwise.
             */
            [[nodiscard]] constexpr bool ShouldReplicate() const noexcept;

            std::size_t size = 0;

            [[nodiscard]] constexpr MGE::Hash GetHash() const noexcept;

        private:
            const std::string name;
            std::vector<Property> properties;
            std::vector<RPC> rpcs;
    };

    std::ostream &operator<<(std::ostream &os, const Class& _class);

    constexpr Class::Class(std::string name) : name(std::move(name)) {}

    constexpr const std::string &Class::GetName() const noexcept
    {
        return name;
    }

    constexpr bool Class::ShouldReplicate() const noexcept
    {
        return std::ranges::any_of(properties, [](const Property& property) -> bool {
            return property.ShouldReplicate();
        });
    }

    constexpr const std::vector<Property>& Class::GetProperties() const noexcept
    {
        return properties;
    }

    constexpr const Property &Class::GetProperty(const Hash &propertyHash) const
    {
        const auto& opt = TryGetProperty(propertyHash);

        if (!opt.has_value())
            throw std::out_of_range("The requested property does not exists in " + GetName() + '.');
        return *opt;
    }

    constexpr bool Class::HasProperty(const std::string_view propertyName) const
    {
        return HasProperty(HashString(propertyName));
    }

    constexpr bool Class::HasProperty(const Hash &propertyHash) const
    {
        return std::ranges::find_if(properties, [propertyHash](const Property& property) -> bool {
            return property.GetHash() == propertyHash;
        }) != properties.end();
    }


    constexpr const Property& Class::GetProperty(const std::string_view propertyName) const
    {
        const auto& opt = TryGetProperty(propertyName);

        if (!opt.has_value())
            throw std::out_of_range("Property \"" + std::string(propertyName) + "\" does not exists in " + GetName() + '.');
        return *opt;
    }

    constexpr std::optional<std::reference_wrapper<const Property>> Class::TryGetProperty(const std::string_view propertyName) const
    {
        auto it = std::ranges::find_if(properties, [&propertyName](const Property& property) -> bool {
            return propertyName == property.GetName();
        });

        if (it == properties.end())
            return std::nullopt;
        return *it;
    }

    constexpr std::optional<std::reference_wrapper<const Property>> Class::TryGetProperty(const Hash &propertyHash) const
    {
        auto it = std::ranges::find_if(properties, [propertyHash](const Property& property) -> bool {
            return property.GetHash() == propertyHash;
        });

        if (it == properties.end())
            return std::nullopt;

        return *it;
    }

    constexpr const RPC &Class::GetRPC(const std::string &rpcName) const
    {
        const auto& opt = TryGetRPC(rpcName);

        if (!opt.has_value())
            throw std::out_of_range("RPC " + rpcName + " was not found in " + GetName());
        return *opt;
    }

    constexpr const RPC &Class::GetRPC(const Hash &rpcHash) const
    {
        const auto& opt = TryGetRPC(rpcHash);

        if (!opt.has_value())
            throw std::out_of_range("The requested RPC was not found in " + GetName());
        return *opt;
    }

    constexpr std::optional<std::reference_wrapper<const RPC>> Class::TryGetRPC(const std::string &rpcName) const
    {
        auto it = std::ranges::find_if(rpcs, [&rpcName](const RPC& rpc) -> bool {
            return rpcName == rpc;
        });

        if (it == rpcs.end())
            return std::nullopt;
        return *it;
    }

    constexpr std::optional<std::reference_wrapper<const RPC>> Class::TryGetRPC(const Hash &rpcHash) const
    {
        auto it = std::ranges::find_if(rpcs, [rpcHash](const RPC& rpc) -> bool {
            return rpc.GetHash() == rpcHash;
        });

        if (it == rpcs.end())
            return std::nullopt;

        return *it;
    }

    constexpr const std::vector<RPC> &Class::GetRPCs() const noexcept
    {
        return rpcs;
    }

    constexpr MGE::Hash Class::GetHash() const noexcept
    {
        return MGE::HashString(name);
    }

    constexpr bool operator==(const MGE::Class& a, const MGE::Class& b) noexcept
    {
        return std::addressof(a) == std::addressof(b);
    }
} // MGE

#endif //RTYPE_CLASS_HPP
