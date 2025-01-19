//
// Created by np on 12/4/24.
//

#ifndef RTYPE_RIGIDBODY_HPP
#define RTYPE_RIGIDBODY_HPP

#include "Reflection/Reflection.hpp"
#include "Physics/GravitySettings.hpp"

namespace MGE::Common
{
    /**
     * @struct RigidBodyComponent
     * A component that represents a body that cannot be deformed by interacting with its world.
     */
    struct RigidBodyComponent
    {
        REFLECT_STRUCT(RigidBodyComponent)

        RigidBodyComponent() = default;

        /**
         * Gravity settings of this body.
         */
        GravitySettings settings;
    };
} // MGE

#endif //RTYPE_RIGIDBODY_HPP
