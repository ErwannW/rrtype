//
// Created by Nail on 20/11/2024.
//

#include "ModuleInfo.hpp"

#include <utility>

namespace MGE
{
    ModuleInfo::ModuleInfo(std::string moduleName) : moduleName(std::move(moduleName))
    {

    }

    BuildTarget::BuildConfiguration ModuleInfo::GetBuildConfiguration() const noexcept
    {
        return buildConfiguration;
    }

    const std::string& ModuleInfo::GetName() const noexcept
    {
        return moduleName;
    }
} // MGE