//
// Created by np on 12/10/24.
//

#ifndef FIXEDSIZESTRING_HPP
#define FIXEDSIZESTRING_HPP

#include <cstring>
#include "Types.hpp"

namespace MGE
{
    template<uint16 size>
    struct FixedSizeString
    {
        private:
            char data[size]{};
    };
}

#endif //FIXEDSIZESTRING_HPP
