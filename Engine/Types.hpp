//
// Created by np on 12/3/24.
//

#ifndef RTYPE_TYPES_HPP
#define RTYPE_TYPES_HPP

#if __STDCPP_FLOAT64_T__
#include <stdfloat>
#endif
#include <cstdint>

template<typename TTarget, typename TReturn, typename... TArgs>
using MethodPtr = TReturn (TTarget::*)(TArgs...);

using int8 = std::int8_t;
using uint8 = std::uint8_t;

using int16 = std::int16_t;
using uint16 = std::uint16_t;

using int32 = std::int32_t;
using uint32 = std::uint32_t;

using int64 = std::int64_t;
using uint64 = std::uint64_t;

using float32 =
#if __STDCPP_FLOAT64_T__
    std::float32_t;
#else
    float;
#endif

using float64 =
#if __STDCPP_FLOAT64_T__
    std::float64_t;
#else
    double;
#endif

using ClientId = uint16;

#endif //RTYPE_TYPES_HPP
