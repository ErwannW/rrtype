//
// Created by Nail on 13/12/2024.
//

#ifndef VELOCITYCOMPONENT_HPP
#define VELOCITYCOMPONENT_HPP

#include "Reflection/Reflection.hpp"
#include "Vector/Vector3.hpp"

namespace MGE::Common
{
    /**
     * @struct VelocityComponent
     */
    struct VelocityComponent
    {
        REFLECT_STRUCT(VelocityComponent)

        VelocityComponent() = default;
        explicit VelocityComponent(float x, float y, float z);
        explicit VelocityComponent(Vector3f velocity);

        /**
         * In m/s.
         */
        Vector3f velocity;
    };
} // MGE

#endif //VELOCITYCOMPONENT_HPP
