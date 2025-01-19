//
// Created by Nail on 19/11/2024.
//

#ifndef BITFLAGF_HPP
#define BITFLAGF_HPP

#include <type_traits>

namespace MGE::Bitflag
{
    /**
     *
     * @tparam T The type of the bit flag.
     * @tparam TUnderlying The underlying type of this but flag (useful for enum classes)
     * @param bit The value on which to look for the flag.
     * @param flag The flag to look for in bit.
     * @return true if bit contains the given flag, false otherwise.
     */
    template<typename T, typename TUnderlying = std::underlying_type_t<T>>
    constexpr bool HasFlag(T bit, T flag)
    {
        return (static_cast<TUnderlying>(bit) & static_cast<TUnderlying>(flag)) == static_cast<TUnderlying>(flag);
    }
}

#endif //BITFLAGF_HPP
