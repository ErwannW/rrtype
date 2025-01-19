//
// Created by np on 12/4/24.
//

#ifndef RTYPE_GRAVITYSETTINGS_HPP
#define RTYPE_GRAVITYSETTINGS_HPP

#include "Reflection/Reflection.hpp"
#include "Types.hpp"
#include "Vector/Vector3.hpp"

namespace MGE
{
    /**
     * The source of gravity.
     */
    enum class GravitySource : uint8 {
        /**
         * Gravity is disabled.
         */
        None,
        /**
         * The gravity is a point on the screen.
         */
        Point,
        /**
         * The gravity is a direction.
         */
        Direction
    };

    namespace GravityForce
    {
        /**
         * No gravity.
         */
        constexpr float32 None  = static_cast<float32>(0);
        /**
         * Earth-like gravity force.
         */
        constexpr float32 Earth = static_cast<float32>(5.9722 * 10e24);
        /**
         * Moon-like gravity force.
         */
        constexpr float32 Moon  = static_cast<float32>(7.34767309 * 10e22);
    }

    struct GravitySettings
    {
        REFLECT_STRUCT(GravitySettings)

        /**
         * Kilograms
         */
        float32 mass = 1;

        /**
         * The mass of the object that attracts this rigidbody.
         */
        float32 gravityMass = GravityForce::Earth;

        GravitySource gravitySource = GravitySource::Direction;
        Vector3f gravityDirection = Direction<float32>::Down;
        Vector3f gravityPoint = Direction<float32>::Zero;
    };
} // MGE

#endif //RTYPE_GRAVITYSETTINGS_HPP
