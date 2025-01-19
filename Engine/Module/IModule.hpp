//
// Created by Nail on 20/11/2024.
//

#ifndef IMODULE_HPP
#define IMODULE_HPP

#include "Module/ModuleInfo.hpp"

namespace MGE {
    class Engine;
}

class IModule
{
    public:
        virtual ~IModule() = default;

        /**
         * Returns a ModuleInfo struct that contains basic data about this module.
         */
        [[nodiscard]] virtual const MGE::ModuleInfo &GetModuleInfo() const = 0;

        /**
         * Called when the module has been hooked to the engine.
         * @param engine The engine this module has been hooked to.
         */
        virtual void OnHooked(class Engine &engine) = 0;
};

#endif //IMODULE_HPP
