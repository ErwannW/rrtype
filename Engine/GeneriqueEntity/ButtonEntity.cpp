#include "Registry.hpp"
#include "World.hpp"
#include "GeneriqueComponent.hpp"
#include "Core.hpp"
#include "Random.hpp"
#include "../Commons/Components/Transform.hpp"
/**
 * Creates an Input Entity.
 * @param world The world where the entity will be created.
 * @param spriteID The ID of thesprite to display for the input field.
 * @param textID The ID of the texte to display for the input field.
 * @param text The text displayed on the button.
 * @param x The horizontal position (in percentage) of the center of the input field.
 * @param y The vertical position (in percentage) of the center of the input field.
 */
std::shared_ptr<MGE::Entity> GenericComponent::CreateButton(MGE::World& world, const std::string& spriteID, const std::string& textID, const std::string& text, float x, float y, bool isDisabled, float size, std::function<void()> onClick)
{
    std::shared_ptr<MGE::Entity> button = world.GetRegistry().CreateEntity();

    world.GetRegistry().EmplaceComponent<TextComponent>(*button, TextComponent(text, textID, x, y));
    world.GetRegistry().EmplaceComponent<SpriteComponent>(*button, SpriteComponent(spriteID, size, size));
    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*button, MGE::Common::TransformComponent(MGE::Vector3f(x, y, 0)));
    world.GetRegistry().EmplaceComponent<ButtonComponent>(*button, ButtonComponent(isDisabled, spriteID, spriteID + "_hover", spriteID + "_disable", true, onClick, false));
    world.GetRegistry().EmplaceComponent<MouseComponent>(*button, MouseComponent(0, 0, true, false, false));
    world.GetRegistry().EmplaceComponent<SongComponent>(*button, SongComponent("button_1_hover" , false));

    return button;
}

SongComponent::SongComponent(std::string texture_id, bool isPlaying) : texture_id(std::move(texture_id)), isPlaying(isPlaying)
{

}

void SongComponent::SetupReflection()
{
    Class.RegisterProperty(texture_id);
    Class.RegisterProperty(isPlaying);
}

MouseComponent::MouseComponent(int pos_x, int pos_y, bool displayDefaultMouse, bool LeftButtonPressed, bool RightButtonPressed)
    : pos_x(pos_x), pos_y(pos_y), displayDefaultMouse(displayDefaultMouse),
      LeftButtonPressed(LeftButtonPressed) , RightButtonPressed(RightButtonPressed)
{

}

void MouseComponent::SetupReflection()
{
    Class.RegisterProperty(pos_x);
    Class.RegisterProperty(pos_y);
    Class.RegisterProperty(displayDefaultMouse);
    Class.RegisterProperty(LeftButtonPressed);
    Class.RegisterProperty(RightButtonPressed);
}

SpriteComponent::SpriteComponent(std::string texture_id, float width, float height)
    : texture_id(std::move(texture_id)), width(width), height(height)
{
    this->uuid = UuidGenerator();
}

void SpriteComponent::SetupReflection()
{
    Class.RegisterProperty(texture_id);
    Class.RegisterProperty(width);
    Class.RegisterProperty(height);
}

IdComponent::IdComponent(const int id) : id(id)
{

}

void IdComponent::SetupReflection()
{
    Class.RegisterProperty(id);
}

ButtonComponent::ButtonComponent(bool isDisabled, std::string texture_id_standard, std::string texture_id_hover,
                                 std::string texture_id_disabled, bool mouse_exit_hover, std::function<void()> onClick,
                                 bool isPressed)
    : isDisabled(isDisabled),
      isPressed(isPressed),
      mouse_exit_hover(mouse_exit_hover),
      texture_id_standard(std::move(texture_id_standard)),
      texture_id_hover(std::move(texture_id_hover)),
      texture_id_disabled(std::move(texture_id_disabled)),
      onClick(std::move(onClick))
{

}

void ButtonComponent::SetupReflection()
{
    Class.RegisterProperty(isDisabled);
    Class.RegisterProperty(isPressed);
    Class.RegisterProperty(mouse_exit_hover);
    Class.RegisterProperty(texture_id_standard);
    Class.RegisterProperty(texture_id_hover);
    Class.RegisterProperty(texture_id_disabled);
    Class.RegisterProperty(onClick);
}

TextComponent::TextComponent(std::string text, std::string texture_id, float position_x, float position_y, bool is_input, int textMaxSize, int display_position)
    : text(std::move(text)), texture_id(std::move(texture_id)), position_x(position_x), position_y(position_y), is_input(is_input), textMaxSize(textMaxSize), display_position(display_position)
{
    this->uuid = UuidGenerator();
}

void TextComponent::SetupReflection()
{
    Class.RegisterProperty(text);
    Class.RegisterProperty(texture_id);
    Class.RegisterProperty(position_x);
    Class.RegisterProperty(position_y);
    Class.RegisterProperty(is_input);
    Class.RegisterProperty(display_position);
    Class.RegisterProperty(textMaxSize);
}

InputComponent::InputComponent(bool isHeld, bool isHovered, std::string previousText)
    : isHeld(isHeld), isHovered(isHovered), previousText(std::move(previousText))
{

}

void InputComponent::SetupReflection()
{
    Class.RegisterProperty(isHeld);
    Class.RegisterProperty(isHovered);
    Class.RegisterProperty(previousText);
}

KeyBoardsComponents::KeyBoardsComponents(std::string input)
    : input(std::move(input))
{

}

void KeyBoardsComponents::SetupReflection()
{
    Class.RegisterProperty(input);
}

SoundComponent::SoundComponent(float volume) : volume(volume)
{

}

void SoundComponent::SetupReflection()
{
    Class.RegisterProperty(volume);
}
