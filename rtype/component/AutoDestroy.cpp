//
// Created by louis on 09/01/2025.
//

#include "SystemeComponent.hpp"

void ComponentRType::AutoDestroySongSysteme(MGE::World &world, MGE::EntityRef& entity, AutoDestroyComponent &autoDestroyComponent, SongComponent &songComponent)
{
    if (!autoDestroyComponent.song)
        return;
    autoDestroyComponent.elapsedTime += world.GetFDeltaSeconds();
    if (autoDestroyComponent.elapsedTime >= autoDestroyComponent.lifeTime) {
        songComponent.isPlaying = false;
    }
}

void ComponentRType::AutoDestroySpriteSysteme(MGE::World &world, MGE::EntityRef& entity, AutoDestroyComponent &autoDestroyComponent, SpriteComponent &spriteComponent)
{
    if (!autoDestroyComponent.sprite)
        return;
    autoDestroyComponent.elapsedTime += world.GetFDeltaSeconds();
    if (autoDestroyComponent.elapsedTime  >= autoDestroyComponent.lifeTime) {
        if (static_cast<bool>(MGE::BuildTarget::BuildType::Client))
            world.GetRegistry().SetEntityEnabled(*entity.entity, false);
        else if (static_cast<bool>(MGE::BuildTarget::BuildType::Server))
            world.GetRegistry().DestroyEntity(entity.entity);
    }
}

AutoDestroyComponent::AutoDestroyComponent(float32 lifeTime, bool sprite, bool song) : lifeTime(lifeTime), sprite(sprite), song(song) {}
void AutoDestroyComponent::SetupReflection()
{}
