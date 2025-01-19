//
// Created by np on 12/4/24.
//

#include "RigidBody.hpp"

namespace MGE::Common
{
    void RigidBodyComponent::SetupReflection()
    {
        Class.RegisterProperty(settings);
    }
} // MGE