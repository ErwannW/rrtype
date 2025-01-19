//
// Created by nailperreau on 25/11/24.
//

#include "SetServerAddr.hpp"
#include "LobbyLevel.hpp"
#include "Settings.hpp"
#include "../../Engine/World.hpp"
#include "../../Engine/Entity.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

namespace MGE
{
    Settings::Settings(World &world) : Level(world)
    {
    }

    void Settings::OnStart()
    {
        Level::OnStart();

        BackgroundImage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();
        ClientVersionText = world.GetRegistry().CreateEntity();
        EngineVersionText = world.GetRegistry().CreateEntity();
        ServerVersionText = world.GetRegistry().CreateEntity();
        Background = world.GetRegistry().CreateEntity();
        ButtonSave = world.GetRegistry().CreateEntity();

        accessibilityModeBouton = world.GetRegistry().CreateEntity();
        songLevelButtonMore = world.GetRegistry().CreateEntity();
        songLevelButtonLess = world.GetRegistry().CreateEntity();
        songLevelText = world.GetRegistry().CreateEntity();
        songLevelTextValue = world.GetRegistry().CreateEntity();

        world.GetRegistry().EmplaceComponent<SpriteComponent>(BackgroundImage, "Loading_Backgroung", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(BackgroundImage, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<SpriteComponent>(Background, "pop_up_background", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(Background, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<TextComponent>(ClientVersionText, "Client version : " + world.ClientVersion, "Version", 90.f, 85.f);
        world.GetRegistry().EmplaceComponent<TextComponent>(EngineVersionText, "Engine version : " + world.EngineVersion, "Version", 90.f, 90.f);
        world.GetRegistry().EmplaceComponent<TextComponent>(ServerVersionText, "Server version : " + world.ServerVersion, "Version", 90.f, 95.f);
        world.GetRegistry().EmplaceComponent<SongComponent>(Song, "Loading_Song", true);

        playerNameInput = generiqueComponent.CreateInput(world, "Input_1", "button_1", "Surname", 30, 50, 50, false, 50, 50,
        [this]() {
        });

        accessibilityModeBouton = generiqueComponent.CreateButton(world, "button_1", "button_1", "Accessibility Mode", 14.75, 35, false, 30,
        [this]() {
            world.engine.GetRenderer<SfmlDisplay>()->SwitchDisplayMode();
        });

        ButtonSave = generiqueComponent.CreateButton(world, "button_1", "button_1", "Validations", 14.75, 80, false, 30,
        [this]() {
            world.SetLevel(std::make_unique<MGE::LobbyLevel>(world));
        });

        ButtonRetour = generiqueComponent.CreateButton(world, "button_1", "button_1", "Back", 14.75, 20, false, 30,
        [this]() {
            world.SetLevel(std::make_unique<MGE::LobbyLevel>(world));
        });

        songLevelButtonLess = generiqueComponent.CreateButton(world, "SoundMinus_button", "button_1", "", 5, 70, false, 6,
            [this]() {
                world.engine.GetRenderer<SfmlDisplay>()->ChangeSoundLevel(-10);
        });

        songLevelButtonMore = generiqueComponent.CreateButton(world, "SoundPlus_button", "button_1", "", 15, 70, false, 6,
            [this]() {
                world.engine.GetRenderer<SfmlDisplay>()->ChangeSoundLevel(10);
        });

        eyeButton = generiqueComponent.CreateButton(world, "eye", "button_1", "", 50, 70, false, 10,
        [this]() {

        });

        world.GetRegistry().EmplaceComponent<TextComponent>(songLevelText, "Sound :", "button_1", 6, 62.5);
        world.GetRegistry().EmplaceComponent<TextComponent>(songLevelTextValue, "50", "button_1", 10, 70);
        world.GetRegistry().EmplaceComponent<SoundComponent>(songLevelTextValue, 50.f);
    }

    std::string_view Settings::GetName() const
    {
        return "Settings";
    }

    void Settings::OnClose()
    {

    }

} // MGE