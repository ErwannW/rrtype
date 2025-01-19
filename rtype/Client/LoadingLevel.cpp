//
// Created by nailperreau on 25/11/24.
//

#include "Core.hpp"
#include "LoadingLevel.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"

namespace MGE
{
    LoadingLevel::LoadingLevel(World &world) : Level(world)
    {
    }

    void LoadingLevel::OnStart()
    {
        Level::OnStart();

        LoadingPage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();
        LoadingAnimation = world.GetRegistry().CreateEntity();
        LoadingText = world.GetRegistry().CreateEntity();
        Background = world.GetRegistry().CreateEntity();

        world.GetRegistry().EmplaceComponent<SpriteComponent>(LoadingPage, "Loading_Backgroung", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(LoadingPage, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<SpriteComponent>(Background, "pop_up_background", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(Background, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<SpriteComponent>(Song, "Loading_animation", 15.f, 15.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(Song, MGE::Common::TransformComponent(MGE::Vector3f(90.f, 90.f, 0)));

        world.GetRegistry().EmplaceComponent<TextComponent>(LoadingText, "Waiting other player ...", "Title", 50.f, 50.f);
        world.GetRegistry().EmplaceComponent<SongComponent>(Song, "Loading_Song", true);
    }

    void LoadingLevel::OnUpdate()
    {
        if (world.gameState == RType::GameState::InGame)
            world.SetLevel(std::make_shared<GameLevel>(world));
    }

    void LoadingLevel::OnClose() {
        Level::OnClose();

        world.engine.GetRenderer<SfmlDisplay>()->StopAllSong();
    }

    std::string_view LoadingLevel::GetName() const
    {
        return "Loading Level";
    }
} // MGE