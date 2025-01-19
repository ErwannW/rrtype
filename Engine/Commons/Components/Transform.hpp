//
// Created by np on 11/27/24.
//

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Core.hpp"

namespace MGE::Common
{
    /**
     * @struct TransformComponent
     * A component that has a position, rotation and scale.
     */
    struct TransformComponent
    {
        REFLECT_STRUCT(TransformComponent)

        Vector3f position = 0;
        Vector3f rotation = 0;
        Vector3f scale = 1;

        TransformComponent() = default;
        explicit TransformComponent(Vector3f position, Vector3f rotation = 0, Vector3f scale = 1)
            : position(position) {}
    };
} // MGE

#endif //TRANSFORM_HPP
