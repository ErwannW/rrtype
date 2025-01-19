//
// Created by np on 11/28/24.
//

#ifndef RTYPE_HASH_HPP
#define RTYPE_HASH_HPP

#include <ostream>
#include <cstdint>

namespace MGE
{
    using HashType = std::uint64_t;

    /**
     * @enum Hash
     * Represents a bits hash.
     */
    enum class Hash : HashType {
        None = 0,
    };

    /**
     * Hashes a string.
     * @param str The string to hash.
     * @return The hashed string.
     */
    constexpr Hash HashString(const std::string_view str) noexcept
    {
        HashType hash = 5381;

        for (const char c : str)
            hash = hash * 33 ^ c;

        return static_cast<Hash>(hash);
    }

    /**
     * @namespace CT
     * Provides guaranteed compile-time hashing functionality.
     */
    namespace CT
    {
        /**
         * Hashes a string.
         * @param str The string to hash.
         * @return The hashed string.
         */
        consteval Hash HashString(const std::string_view str) noexcept
        {
            return MGE::HashString(str);
        }
    }

    std::ostream &operator<<(std::ostream& os, MGE::Hash hash);
} // MGE
#endif //RTYPE_HASH_HPP
