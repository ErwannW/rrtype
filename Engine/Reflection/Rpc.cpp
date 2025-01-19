//
// Created by np on 12/6/24.
//

#include "Rpc.hpp"
#include "Class.hpp"

namespace MGE
{
    void RPC::operator()(void *obj, void *data) const
    {
        return Call(obj, data);
    }

    void RPC::Call(void *obj, const void *data) const
    {
        return call(obj, data);
    }

    MGE::Hash RPC::GetHash() const
    {
        return hash;
    }

    const std::string &RPC::GetName() const
    {
        return name;
    }

    std::string RPC::GetFullName() const
    {
        return _class.GetName() + "::" + GetName();
    }
} // MGE