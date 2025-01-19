//
// Created by Nail on 27/09/2024.
//

#ifndef MGE_RANDOM_HPP
#define MGE_RANDOM_HPP

#include <cstdint>
#include <string>
#include <iomanip>
#include <random>

namespace MGE
{
    class Random
    {
        public:
            Random(int seed);

            template<typename TInteger>
            TInteger Integer(const TInteger min = std::numeric_limits<TInteger>::min(), const TInteger max = std::numeric_limits<TInteger>::max());

        private:
            std::default_random_engine engine;
    };

    template <typename TInteger>
    inline TInteger Random::Integer(const TInteger min, const TInteger max)
    {
        static_assert(std::is_integral<TInteger>::value, "TInteger must be an integral type.");

        auto distrib = std::uniform_int_distribution<TInteger>(min, max);
        return distrib(engine);
    }

}

std::string UuidGenerator();

#endif //MGE_RANDOM_HPP
