//
// Created by np on 11/27/24.
//

#include "Collider.hpp"

namespace MGE::Common
{
    void BoxColliderComponent::SetupReflection()
    {
        Class.RegisterProperty(boundingBox);
    }

    void SphereColliderComponent::SetupReflection()
    {
        Class.RegisterProperty(radius);
    }
}