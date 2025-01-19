//
// Created by louis on 18/01/25.
//

#include "SystemeComponent.hpp"

void ComponentRType::LifeSystem(MGE::World &world, MGE::EntityRef& entity,  LifeComponent &lifeComponent)
{
    if (world.CurrentLife <= 2 && lifeComponent.lifeNumber == 3)
        world.GetRegistry().SetEntityEnabled(*entity.entity, false);
    if (world.CurrentLife <= 1 && lifeComponent.lifeNumber == 2)
        world.GetRegistry().SetEntityEnabled(*entity.entity, false);
    if (world.CurrentLife <= 0 && lifeComponent.lifeNumber == 1)
        world.GetRegistry().SetEntityEnabled(*entity.entity, false);
}

LifeComponent::LifeComponent(int lifeNumber) : lifeNumber(lifeNumber){}

void LifeComponent::SetupReflection()
{
}