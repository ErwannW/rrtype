//
// Created by Nail on 13/12/2024.
//

#include "VelocityComponent.hpp"

namespace MGE::Common
{
    void VelocityComponent::SetupReflection()
    {
        Class.RegisterProperty(velocity);
    }

    VelocityComponent::VelocityComponent(const float x, const float y, const float z) : velocity(x, y, z)
    {
    }

    VelocityComponent::VelocityComponent(const Vector3f velocity) : velocity(velocity)
    {
    }
} // MGE