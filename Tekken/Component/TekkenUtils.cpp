/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** component-r-type
*/

// LOCAL INCLUDES
#include "ComponentTekken.hpp"


bool ComponentRType::CollisionCircle(MGE::Vector3f positionProjectile, MGE::Vector3f positionCible, float collisionSize)
{
    double dx = positionCible.x - positionProjectile.x;
    double dy = positionCible.y - positionProjectile.y;
    double distanceCarre = dx * dx + dy * dy;
    double rayonCarre = collisionSize * collisionSize;

    if (distanceCarre <= rayonCarre)
        return true;
    return false;
}





