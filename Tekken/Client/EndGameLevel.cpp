// ENGINE INCLUDES
#include "../../Engine/World.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

// LOCAL INCLUDES
#include "LobbyLevel.hpp"
#include "LoadingLevel.hpp"
#include "EndGameLevel.hpp"

namespace MGE
{
    EndGameLevel::EndGameLevel(World &world) : Level(world) {}

    void EndGameLevel::OnStart()
    {
        Level::OnStart();

        BackgroundImage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();
        Background = world.GetRegistry().CreateEntity();
        EndGameText = world.GetRegistry().CreateEntity();
        world.GetRegistry().EmplaceComponent<SpriteComponent>(BackgroundImage, "TekkenSpriteMenu", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(BackgroundImage, Common::TransformComponent(Vector3f(50.f, 50.f, 0)));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(Background, "pop_up_background", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(Background, Common::TransformComponent(Vector3f(50.f, 50.f, 0)));

        switch (world.gameState) {
            case RType::GameState::GameWin:
                world.GetRegistry().EmplaceComponent<TextComponent>(EndGameText, "You win this Stage", "Title", 50.f, 50.f);
                world.GetRegistry().EmplaceComponent<SongComponent>(Song, "game_win", true);
                NextStageButton = generiqueComponent.CreateButton(world, "button_1", "button_1", "Next Stage", 14.75f, 60.f, false, 30.f,
                  [this] {
                  network.SendRPCGameEventFormClient(RType::EventType::NextStage, world);
                  world.SetLevel(std::make_shared<LoadingLevel>(world));
                });
                break;
            case RType::GameState::GameOver:
                world.GetRegistry().EmplaceComponent<TextComponent>(EndGameText, "Game Over", "Title", 50.f, 50.f);
                world.GetRegistry().EmplaceComponent<SongComponent>(Song, "game_loos", true);
                NextStageButton = generiqueComponent.CreateButton(world, "button_1", "button_1", "Next Stage", 14.75f, 60.f, false, 30.f,
                [this] {
                  network.SendRPCGameEventFormClient(RType::EventType::InGame, world);
                  world.SetLevel(std::make_shared<LoadingLevel>(world));
                });
                break;
            case RType::GameState::Error:
                world.GetRegistry().EmplaceComponent<TextComponent>(EndGameText, "An error occur with the server", "Title", 50.f, 50.f);
                world.GetRegistry().EmplaceComponent<SongComponent>(Song, "TekkenSongMenu", true);
                break;
            default:
                break;
        }
        LobbyButton = generiqueComponent.CreateButton(world, "button_1", "button_1", "Exit", 14.75f, 80.f, false, 30.f,
        [this] {
            world.SetLevel(std::make_unique<LobbyLevel>(world));
        });
    }

    void EndGameLevel::OnClose() {
        Level::OnClose();

        world.engine.GetRenderer<SfmlDisplay>()->StopAllSong();
    }


    std::string_view EndGameLevel::GetName() const
    {
        return "end game level";
    }
} // MGE