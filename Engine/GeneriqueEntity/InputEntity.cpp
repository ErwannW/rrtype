#include "Core.hpp"
#include "GeneriqueComponent.hpp"
#include <string>
#include "../Commons/Components/Transform.hpp"

/**
 * Creates an Input Entity.
 * @param world The world where the entity will be created.
 * @param spriteID The ID of the sprite to display for the input field.
 * @param textID The ID of the texte to display for the input field.
 * @param text The default text displayed in the input field.
 * @param textMaxSize maximu text size
 * @param x The horizontal position (in percentage) of the center of the input field.
 * @param y The vertical position (in percentage) of the center of the input field.
 */
std::shared_ptr<MGE::Entity>
GenericComponent::CreateInput(MGE::World &world,
    const std::string &spriteID, const std::string &textID,
    const std::string &text, int textMaxSize, float size_x,
    float size_y, bool isDisabled, float pos_x, float pos_y,
    std::function<void()> onClick)
{
    std::shared_ptr<MGE::Entity> input = world.GetRegistry().CreateEntity();

    world.GetRegistry().EmplaceComponent<SpriteComponent>(*input, SpriteComponent(spriteID, size_x, size_y));
    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*input, MGE::Common::TransformComponent(MGE::Vector3f(pos_x, pos_y, 0)));
    world.GetRegistry().EmplaceComponent<InputComponent>(*input, InputComponent(false, false));
    world.GetRegistry().EmplaceComponent<ButtonComponent>(*input, ButtonComponent(isDisabled, spriteID, spriteID + "_hover", spriteID + "_disable", true, onClick, false));
    world.GetRegistry().EmplaceComponent<TextComponent>(*input, TextComponent(text, textID, pos_x, pos_y, false, textMaxSize, -1));
    world.GetRegistry().EmplaceComponent<MouseComponent>(*input, MouseComponent(0, 0, true, false, false));
    world.GetRegistry().EmplaceComponent<KeyBoardsComponents>(*input, KeyBoardsComponents());

    return input;
}
