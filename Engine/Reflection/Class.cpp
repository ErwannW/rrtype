//
// Created by np on 12/2/24.
//

#include "Class.hpp"

namespace MGE
{
    std::ostream &operator<<(std::ostream &os, const Class& _class)
    {
        return os << _class.GetName();
    }
} // MGE