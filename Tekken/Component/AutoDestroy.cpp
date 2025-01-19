//
// Created by louis on 09/01/2025.
//

#include "ComponentTekken.hpp"

void ComponentRType::AutoDestroySysteme(MGE::World &world, MGE::EntityRef& entity, AutoDestroyComponent &autoDestroyComponent)
{
    autoDestroyComponent.elapsedTime += world.GetFDeltaSeconds();
    if (autoDestroyComponent.elapsedTime * 10 >= autoDestroyComponent.lifeTime) {
        auto songComponent = world.GetRegistry().GetComponentAddress<SongComponent>(*entity.entity);
        if (songComponent != nullptr)
            songComponent->isPlaying = false;
    }
}

AutoDestroyComponent::AutoDestroyComponent(float32 lifeTime) : lifeTime(lifeTime) {}

void AutoDestroyComponent::SetupReflection()
{}
