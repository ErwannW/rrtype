//
// Created by Nail on 20/11/2024.
//

#ifndef MODULEINFO_HPP
#define MODULEINFO_HPP

#include <string>
#include <BuildTarget.hpp>

namespace MGE
{
    struct ModuleInfo
    {
        explicit ModuleInfo(std::string moduleName);

        [[nodiscard]] BuildTarget::BuildConfiguration GetBuildConfiguration() const noexcept;
        [[nodiscard]] const std::string &GetName() const noexcept;

        private:
            BuildTarget::BuildConfiguration buildConfiguration = BuildTarget::CurrentBuildConfiguration;
            std::string moduleName;
    };
} // MGE

#endif //MODULEINFO_HPP
