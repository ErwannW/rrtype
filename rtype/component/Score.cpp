//
// Created by louis on 18/01/25.
//

#include "SystemeComponent.hpp"

void ComponentRType::ScoreSysteme(MGE::World &world, MGE::EntityRef& entity, ScoreComponent &scoreComponent, TextComponent &textComponent)
{
    textComponent.text = "Score: " + std::to_string(world.score);
}

ScoreComponent::ScoreComponent() {};

void ScoreComponent::SetupReflection()
{
}