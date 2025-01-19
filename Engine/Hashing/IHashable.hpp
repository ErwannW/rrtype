//
// Created by np on 11/28/24.
//

#ifndef IHASHABLE_HPP
#define IHASHABLE_HPP

#include <cstdint>
#include <ostream>
#include "Hash.hpp"

namespace MGE
{
    /**
     * Interface for objects that can be represented by a hash.
     */
    class IHashable
    {
        public:
            virtual ~IHashable() = default;

            /**
             * @return The hash of this object.
             */
            [[nodiscard]] virtual Hash GetHash() const = 0;

            /**
             * Compares two objects by their hash.
             * @param lhs
             * @param rhs
             * @return true if the hash of two objects are the same, false otherwise.
             */
            [[nodiscard("Comparing hashes and discarding the result has no effect.")]] static bool CompareHash(const IHashable& lhs, const IHashable& rhs);
    };
} // MGE

#endif //IHASHABLE_HPP
