//
// Created by np on 11/29/24.
//

#ifndef RTYPE_PROPERTY_HPP
#define RTYPE_PROPERTY_HPP

#include <string>
#include <cstdint>
#include <typeindex>
#include <cstring>
#include <functional>
#include "Hashing/Hash.hpp"
#include "Asserts.hpp"
#include "Types.hpp"
#include "Bitflag.hpp"

namespace MGE
{
    /**
     * @struct Property
     * Represents a property of a class for the reflection system.
     */
    struct Property
    {
        /**
         * Additional attributes of a property.
         */
        enum class Attributes : uint8 {
            None       = 0b00000000,
            Replicated = 0b00000001,
        };

        /**
         * Creates a property.
         * @tparam TTarget The class of this property.
         * @tparam TProperty The type fo this property.
         * @param _class The class of this property.
         * @param name The name of this property.
         * @param member The member pointer of this property.
         * @param attributes The attributes of this property.
         */
        template<typename TTarget, typename TProperty>
        Property(const class Class& _class, std::string name, TProperty TTarget::*member, Attributes attributes = Attributes::None);

        /**
         * The class of this property. This is not the type of this property.
         */
        const Class& _class;

        /**
         * @return The full name of this property ("ClassName::PropertyName").
         */
        [[nodiscard]] std::string GetFullName() const;

        /**
         * @return The size of this property.
         */
        [[nodiscard]] constexpr std::size_t GetSize() const noexcept;

        /**
         * @return The name of this property.
         */
        [[nodiscard]] constexpr const std::string& GetName() const noexcept;

        /**
         * @return The hash of the property. Note that the hash of the property is computed using its full name rather than just its name.
         */
        [[nodiscard]] constexpr MGE::Hash GetHash() const noexcept;

        /**
         * @return The std::type_index of this property.
         */
        [[nodiscard]] std::type_index GetTypeIndex() const noexcept;

        /**
         * @return The address of this property on obj.
         */
        void* GetAddress(void* obj) const;
        const void* GetAddress(const void* obj) const;

        /**
         * @tparam T The type of the property.
         * @return A reference to this property.
         */
        template<typename T>
        [[nodiscard]] T& GetRef(void* obj) const;

        /**
         * Set the value of this property on obj.
         * @param value A pointer to the value to assign.
         */
        void SetValue(void* obj, const void *value) const;

        /**
         * @return true if this property is marked as replicated, false otherwise.
         */
        [[nodiscard]] constexpr bool ShouldReplicate() const noexcept;

        /**
         * Check if this property has an attribute.
         * @param attribute The attribute to look for.
         * @return true if this property has the given attribute, false otherwise.
         */
        [[nodiscard]] constexpr bool HasAttribute(Attributes attribute) const noexcept;

        private:
            std::size_t size;
            std::string name;
            MGE::Hash hash;
            std::type_index typeIndex;

            std::function<void*(void*)> getFunc;
            std::function<void(void*, const void* value)> setFunc;

            Attributes attributes = Attributes::None;
    };

    std::ostream &operator<<(std::ostream &os, const Property& property);

    constexpr std::size_t Property::GetSize() const noexcept
    {
        return size;
    }

    constexpr const std::string &Property::GetName() const noexcept
    {
        return name;
    }

    constexpr Hash Property::GetHash() const noexcept
    {
        return hash;
    }

    constexpr bool Property::HasAttribute(Property::Attributes attribute) const noexcept
    {
        return Bitflag::HasFlag(attributes, attribute);
    }

    constexpr bool Property::ShouldReplicate() const noexcept
    {
        return HasAttribute(Attributes::Replicated);
    }

    /**
     * @return A reference to the given property of obj. No type checking is made, don't fuck things up!
     */
    template<typename T>
    T &Property::GetRef(void *obj) const
    {
        if (typeid(T) != typeIndex)
            throw std::bad_cast();
        return *static_cast<T*>(GetAddress(obj));
    }

    template<typename TTarget, typename TProperty>
    Property::Property(const Class& _class, std::string name, TProperty TTarget::*member, const Attributes attributes)
        : _class(_class), size(sizeof(TProperty)), name(std::move(name)), hash(MGE::HashString(GetFullName())),
        typeIndex(typeid(TProperty)),
        getFunc([member](void* target) -> void* {
            return std::addressof(static_cast<TTarget*>(target)->*member);
        }),
        setFunc([member, this](void* target, const void* value) -> void {
            static_cast<TTarget*>(target)->*member = *static_cast<const TProperty*>(value);
        }),
        attributes(attributes)
    {
        if (ShouldReplicate())
            EXPECT(std::is_trivially_copyable_v<TProperty>, GetFullName() + ": attribute " + stringize(Attributes::Replicated) + " is only valid for trivially copyable types.");
    }

    constexpr bool operator==(const Property& a, const Property& b) noexcept
    {
        return std::addressof(a) == std::addressof(b);
    }

    constexpr bool operator==(const Property& property, const std::string& name) noexcept
    {
        return name == property.GetName() || name == property.GetFullName();
    }
}

#endif //RTYPE_PROPERTY_HPP
