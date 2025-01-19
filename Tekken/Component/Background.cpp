//
// Created by louis on 09/01/2025.
//

#include "ComponentTekken.hpp"

void ComponentRType::BackgroundSystem(MGE::World &world, BackgroundComponent &backgroundComponent, MGE::Common::TransformComponent &transformComponent)
{
    if (constexpr float spriteWidth = 185.0f; transformComponent.position.x + spriteWidth <= 0.0f)
        transformComponent.position.x += spriteWidth * 2;
    transformComponent.position.x -= backgroundComponent.scroll_speed * world.GetFDeltaSeconds();
}

BackgroundComponent::BackgroundComponent(float scroll_speed) : scroll_speed(scroll_speed), is_paralax(false) {}

void BackgroundComponent::SetupReflection()
{
    Class.RegisterProperty(scroll_speed);
    Class.RegisterProperty(is_paralax);
}
