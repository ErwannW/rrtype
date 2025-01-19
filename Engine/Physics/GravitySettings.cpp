//
// Created by np on 12/4/24.
//

#include "GravitySettings.hpp"

namespace MGE
{
    void GravitySettings::SetupReflection()
    {
        Class.RegisterProperty(mass);
        Class.RegisterProperty(gravityMass);
        Class.RegisterProperty(gravitySource);
        Class.RegisterProperty(gravityDirection);
        Class.RegisterProperty(gravityPoint);
    }
} // MGE