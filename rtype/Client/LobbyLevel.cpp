//
// Created by nailperreau on 25/11/24.
//

#include "LobbyLevel.hpp"

#include "../../Engine/World.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"
#include "RomSet.hpp"
#include "LoadingLevel.hpp"
#include "SetServerAddr.hpp"
#include "Settings.hpp"
#include "../component/SystemeComponent.hpp"
#include "../Systems/Systems.h"

namespace MGE
{
    LobbyLevel::LobbyLevel(World &world) : Level(world)
    {

    }

    void LobbyLevel::OnStart()
    {
        Level::OnStart();
        StartingLevelTime = world.GetCurrentFrame() ;
        LobbyPage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();

        world.GetRegistry().EmplaceComponent<SpriteComponent>(LobbyPage, "Loading_Backgroung", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(LobbyPage, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<SongComponent>(Song, "Loading_Song", true);

        world.GetRegistry().AddSystem(RType::SpaceshipSystem);

        ButtonServer = generiqueComponent.CreateButton(world, "button_1", "button_1", "Server", 14.75f, 16.67f, false, 30.f,
        [this]() {
            if (world.GetCurrentFrame() - StartingLevelTime > 30)
                world.SetLevel(std::make_shared<SetServerAddr>(world));
        });
        ButtonRoom = generiqueComponent.CreateButton(world, "button_1", "button_1", "Room", 14.75f, 33.34f, true, 30.f,
        [this]() {
            if (world.GetCurrentFrame() - StartingLevelTime > 30)
                world.SetLevel(std::make_shared<RomSet>(world));
        });
        ButtonStart = generiqueComponent.CreateButton(world, "button_1", "button_1", "Start", 14.75f, 50.f, true, 30.f,
        [this]() {
            if (world.GetCurrentFrame() - StartingLevelTime > 30) {
                network.SendRPCGameEventFormClient(RType::EventType::InGame, world);
                world.SetLevel(std::make_shared<LoadingLevel>(world));
            }
        });
        ButtonSetting = generiqueComponent.CreateButton(world, "button_1", "button_1", "Setting", 14.75f, 66.67f, false, 30.f,
        [this]() {
            if (world.GetCurrentFrame() - StartingLevelTime > 30)
                world.SetLevel(std::make_shared<Settings>(world));
        });
        ExitButton = generiqueComponent.CreateButton(world, "button_1", "button_1", "Exit", 14.75f, 83.34f, false, 30.f,
        [this]() {
            if (world.GetCurrentFrame() - StartingLevelTime > 30)
                exit(0);
        });
    }

    void LobbyLevel::OnUpdate() {
      Level::OnUpdate();

      if (world.client->IsConnected()) {
        auto &buttonRoom = world.GetRegistry().GetComponent<ButtonComponent>(*ButtonRoom);
        buttonRoom.isDisabled = false;

        auto &buttonStart = world.GetRegistry().GetComponent<ButtonComponent>(*ButtonStart);
        buttonStart.isDisabled = false;
      }

    }

    void LobbyLevel::OnClose() {
        Level::OnClose();
        world.engine.GetRenderer<SfmlDisplay>()->StopAllSong();
    }

    std::string_view LobbyLevel::GetName() const
    {
        return "Lobby Level";
    }
} // MGE