//
// Created by Nail on 19/11/2024.
//

#include <iostream>
#include <utility>
#include <string>
#include "Core.hpp"
#include "Engine.hpp"
#include "Commons/Systems.hpp"
#include "display/IDisplay.hpp"
#include "System/System.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"
#include "Level/Level.hpp"
#include "../rtype/Client/LobbyLevel.hpp"
#include "Reflection/Class.hpp"
#include "Vector/Vector3.hpp"
#include "Network/ObjectReplicator.hpp"
#include "Network/Packets/NetPacket.hpp"
#include "../rtype/server/ServerLobby.hpp"
#include "Vector/Vector3.hpp"

#include <BuildTarget.hpp>

/**
 * Mouse position system.
 * @param world The world we are working on.
 */
static void MousePosSystem(MGE::World &world, MouseComponent &mouseConponent)
{
    std::pair<float, float> mousePos = world.engine.GetRenderer<IDisplay>()->getMousePosInPercent();
    mouseConponent.pos_x = static_cast<int>(mousePos.first);
    mouseConponent.pos_y = static_cast<int>(mousePos.second);

    mouseConponent.LeftButtonPressed = world.engine.GetRenderer<IDisplay>()->IsMouseLeftPressed();
    mouseConponent.RightButtonPressed = world.engine.GetRenderer<IDisplay>()->IsMouseRightPressed();
}

static void BackgroundSystem(MGE::World &world, BackgroundComponent &backgroundComponent, MGE::Common::TransformComponent &transformComponent)
{
  transformComponent.position.x -= backgroundComponent.scroll_speed * world.GetFDeltaSeconds();
}

/**
 * Song system.
 * @param world The world we are working on.
 * @param songComponent The song component.
 */
static void SongSystem(MGE::World &world, SongComponent &songComponent)
{
    world.engine.GetRenderer<SfmlDisplay>()->PlaySong(songComponent.texture_id, songComponent.isPlaying);
}

/**
 * Sound Text system.
 *
 */
static void SoundSystem(MGE::World &world, TextComponent &textComponent, SoundComponent &soundComponent)
{
    soundComponent.volume = world.engine.GetRenderer<SfmlDisplay>()->GetSoundLevel();
    textComponent.text = std::to_string(static_cast<int>(soundComponent.volume));
}

/**
 * Text system.
 * @param world The world we are working on.
 * @param textComponent The text component.
 */
static void TextSystem(MGE::World &world, TextComponent &textComponent)
{
    world.engine.GetRenderer<SfmlDisplay>()->DrawText(textComponent);
}

/**
 * Text system.
 * @param world The world we are working on.
 * @param textComponent The text component.
 */
static void TextInput(MGE::World &world, InputComponent &inputComponent, TextComponent &textComponent,
                      ButtonComponent &buttonComponent, MouseComponent &mouseConponent,
                      SpriteComponent &spriteComponent, MGE::Common::TransformComponent& transform)
{
    if (!textComponent.is_input) {
        textComponent.is_input = true;
    }

    if (buttonComponent.isDisabled) {
        spriteComponent.texture_id = buttonComponent.texture_id_disabled;
        return;
    }

    bool isHovering = world.engine.GetRenderer<SfmlDisplay>()->buttonGestion(
        transform.position.x, transform.position.y,
        spriteComponent.texture_id, static_cast<float>(mouseConponent.pos_x), static_cast<float>(mouseConponent.pos_y),
        spriteComponent.width, spriteComponent.height
    );

    if (isHovering) {
        spriteComponent.texture_id = buttonComponent.texture_id_hover;

        if (!buttonComponent.mouse_exit_hover) {
            buttonComponent.mouse_exit_hover = true;
            return;
        }

        if (mouseConponent.RightButtonPressed) {
            buttonComponent.isPressed = true;
        }
    } else if (!buttonComponent.isPressed) {
        buttonComponent.mouse_exit_hover = false;
        spriteComponent.texture_id = buttonComponent.texture_id_standard;
    }

    if (buttonComponent.isPressed && mouseConponent.RightButtonPressed == false) {
        if (mouseConponent.LeftButtonPressed) {
            buttonComponent.isPressed = false;
            spriteComponent.texture_id = buttonComponent.texture_id_standard;
        } else {
            spriteComponent.texture_id = buttonComponent.texture_id_hover;
            world.engine.GetRenderer<SfmlDisplay>()->getText(inputComponent.previousText, textComponent);
        }
    }
}


static void ButtonSystem(MGE::World &world, SpriteComponent &spriteComponent, ButtonComponent &buttonComponent,
                         MouseComponent &mouseConponent, SongComponent &songComponent, MGE::Common::TransformComponent& transform)
{
    if (buttonComponent.isDisabled) {
        spriteComponent.texture_id = buttonComponent.texture_id_disabled;
        return;
    }
    if (world.engine.GetRenderer<SfmlDisplay>()->buttonGestion(transform.position.x, transform.position.y, spriteComponent.texture_id, static_cast<float>(mouseConponent.pos_x), static_cast<float>(mouseConponent.pos_y), spriteComponent.width, spriteComponent.height)) {
        spriteComponent.texture_id = buttonComponent.texture_id_hover;
        if (buttonComponent.mouse_exit_hover == true) {
            buttonComponent.mouse_exit_hover = false;
            songComponent.isPlaying = true;
            return;
        } else {
            songComponent.isPlaying = false;
        }

        if (mouseConponent.RightButtonPressed == true) {
            buttonComponent.isPressed = true;
            return;
        }
        if (mouseConponent.RightButtonPressed == false && buttonComponent.isPressed == true) {
            buttonComponent.isPressed = false;
            buttonComponent.onClick();
        }
        return;
    } else {
        buttonComponent.isPressed = false;
    }
    buttonComponent.mouse_exit_hover = true;
    songComponent.isPlaying = false;
    spriteComponent.texture_id = buttonComponent.texture_id_standard;
}

void RegisterSystems(MGE::Engine &engine)
{
    std::shared_ptr<MGE::System> songSystem = std::make_shared<MGE::System>("Song", SongSystem, MGE::System::ClientOnly, 4);
    engine.world->GetRegistry().AddSystem(songSystem);

    std::shared_ptr<MGE::System> inputSystem = std::make_shared<MGE::System>("Text Input", TextInput, MGE::System::ClientOnly, 5);
    engine.world->GetRegistry().AddSystem(inputSystem);

    std::shared_ptr<MGE::System> textSystem = std::make_shared<MGE::System>("Text", TextSystem, MGE::System::ClientOnly, 1);
    engine.world->GetRegistry().AddSystem(textSystem);

    std::shared_ptr<MGE::System> mousePosSystem = std::make_shared<MGE::System>("Mouse Pos", MousePosSystem, MGE::System::ClientOnly);
    engine.world->GetRegistry().AddSystem(mousePosSystem);

    std::shared_ptr<MGE::System> buttonSystem = std::make_shared<MGE::System>("Button", ButtonSystem, MGE::System::ClientOnly);
    engine.world->GetRegistry().AddSystem(buttonSystem);

    std::shared_ptr<MGE::System> Background = std::make_shared<MGE::System>("Background", BackgroundSystem, MGE::System::ClientOnly);
    engine.world->GetRegistry().AddSystem(Background);

    std::shared_ptr<MGE::System> soundText = std::make_shared<MGE::System>("SoundText", SoundSystem, MGE::System::ClientOnly);
    engine.world->GetRegistry().AddSystem(soundText);

    MGE::Common::SpriteSystem->order = 3;
    engine.world->GetRegistry().AddSystem(MGE::Common::SpriteSystem);
    engine.world->GetRegistry().AddSystem(MGE::Common::VelocitySystem);
}

int main(int argc, char** argv)
{
    MGE::NetRole role;

    if constexpr (MGE::BuildTarget::CurrentBuildType == MGE::BuildTarget::BuildType::Client)
        role = MGE::NetRole::Client;
    else if constexpr (MGE::BuildTarget::CurrentBuildType == MGE::BuildTarget::BuildType::Server)
        role = MGE::NetRole::Host;

    MGE::Engine engine = MGE::Engine();
    engine.SetRenderer<SfmlDisplay>();

    if constexpr (MGE::BuildTarget::IsClientBuild())
        engine.world->SetLevel(std::make_shared<MGE::LobbyLevel>(*engine.world));
    else if constexpr (MGE::BuildTarget::IsServerBuild())
        engine.world->SetLevel(std::make_shared<MGE::Server::ServerLobby>(*engine.world));

    RegisterSystems(engine);
    return engine.Start();
}
