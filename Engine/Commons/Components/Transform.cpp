//
// Created by np on 12/3/24.
//

#include "Transform.hpp"

namespace MGE::Common
{
    void TransformComponent::SetupReflection()
    {
        Class.RegisterProperty(position, REPLICATED);
        Class.RegisterProperty(rotation, REPLICATED);
        Class.RegisterProperty(scale   , REPLICATED);
    }
}