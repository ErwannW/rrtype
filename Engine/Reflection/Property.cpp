//
// Created by np on 11/29/24.
//

#include "Property.hpp"
#include "Class.hpp"

namespace MGE
{
    std::string Property::GetFullName() const
    {
        return _class.GetName() + "::" + GetName();
    }

    void *Property::GetAddress(void *obj) const
    {
        return getFunc(obj);
    }

    void Property::SetValue(void *obj, const void *value) const
    {
        return setFunc(obj, value);
    }

    std::ostream &operator<<(std::ostream &os, const Property& property)
    {
        return os << property.GetName();
    }

    std::type_index Property::GetTypeIndex() const noexcept
    {
        return typeIndex;
    }
}

