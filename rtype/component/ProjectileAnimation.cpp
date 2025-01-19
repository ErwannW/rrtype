//
// Created by louis on 18/01/25.
//

#include "SystemeComponent.hpp"

void ComponentRType::ProjectileAnimationsSystemServer(MGE::World &world, ProjectileAnimationsComponent &projectileAnimationsComponent, SpriteComponent &spriteComponent)
{
    projectileAnimationsComponent.elapsedTime += world.GetFDeltaSeconds();
    if (projectileAnimationsComponent.elapsedTime >= projectileAnimationsComponent.lifeTime) {
        spriteComponent.texture_id = projectileAnimationsComponent.NewTextureID;
    }
}

ProjectileAnimationsComponent::ProjectileAnimationsComponent(float32 lifeTime, std::string NewTextureID) : lifeTime(lifeTime), NewTextureID(NewTextureID)  {}

void ProjectileAnimationsComponent::SetupReflection()
{
}

