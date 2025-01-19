//
// Created by np on 11/27/24.
//

#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "Reflection/Reflection.hpp"
#include "Vector/Vector3.hpp"

namespace MGE::Common
{
    /**
     * @struct BoxColliderComponent
     * A collider that has the shape of a box.
     */
    struct BoxColliderComponent
    {
        REFLECT_STRUCT(BoxColliderComponent)

        /**
         * The bounding box of the collider.
         * It represents its height, width and depth.
         */
        Vector3f boundingBox;
    };

    /**
     * @struct SphereColliderComponent
     * A collider that has the shape of a sphere.
     */
    struct SphereColliderComponent
    {
        REFLECT_STRUCT(SphereColliderComponent)

        SphereColliderComponent() = default;

        /**
         * The radius of the sphere.
         */
        float32 radius = 1;
    };
}

#endif //COLLIDER_HPP
