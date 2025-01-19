//
// Created by Nail on 21/11/2024.
//

#include "System.hpp"

#include "Asserts.hpp"

namespace MGE
{
    void System::operator()(Registry &r) const
    {
        if (!enabled)
            return;

        if (CanRunOn(r.world.engine))
            return func(r);
    }

    std::string_view System::GetName() const
    {
        return name;
    }

    bool System::CanRunOn(const Engine &engine) const
    {
        if constexpr (BuildTarget::IsClientBuild())
            return runPolicy.client;
        if constexpr (BuildTarget::IsServerBuild())
            return runPolicy.server;

        return false;
    }

    const void* System::TargetSystemFunctionPtr() const noexcept
    {
        return funcAddr;
    }

    const Query & System::GetQuery() const
    {
        return query;
    }
} // MGE