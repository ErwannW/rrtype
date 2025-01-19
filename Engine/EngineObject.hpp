//
// Created by nailperreau on 25/11/24.
//

#ifndef ENGINEOBJECT_HPP
#define ENGINEOBJECT_HPP

#include <string_view>

#include "Hashing/IHashable.hpp"

namespace MGE
{
    class IEngineObject
    {
        public:
            virtual ~IEngineObject() = default;

            [[nodiscard]] virtual std::string_view GetName() const = 0;
    };
}

#endif //ENGINEOBJECT_HPP
