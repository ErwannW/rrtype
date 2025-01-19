#include "EndGameLevel.hpp"
#include "LobbyLevel.hpp"

#include "World.hpp"
#include "LoadingLevel.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"
#include "CreditLevel.hpp"

namespace MGE
{
    EndGameLevel::EndGameLevel(World &world) : Level(world)
    {
    }

    void EndGameLevel::OnStart()
    {
        Level::OnStart();

        BackgroundImage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();
        Background = world.GetRegistry().CreateEntity();
        EndGameText = world.GetRegistry().CreateEntity();
        world.GetRegistry().EmplaceComponent<SpriteComponent>(BackgroundImage, "Loading_Backgroung", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(BackgroundImage, Common::TransformComponent(Vector3f(50.f, 50.f, 0)));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(Background, "pop_up_background", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(Background, Common::TransformComponent(Vector3f(50.f, 50.f, 0)));

        switch (world.gameState) {
            case RType::GameState::GameWin:
                world.GetRegistry().EmplaceComponent<TextComponent>(EndGameText, "You win this Stage", "Title", 50.f, 50.f);
                world.GetRegistry().EmplaceComponent<SongComponent>(Song, "game_win", true);
                NextStageButton = generiqueComponent.CreateButton(world, "button_1", "button_1", "Next Stage", 14.75f, 60.f, false, 30.f,
                  [this] {
                  RType::RtypeNetwork::SendRPCGameEventFormClient(RType::EventType::NextStage, world);
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
                world.GetRegistry().EmplaceComponent<SongComponent>(Song, "Loading_Song", true);
                break;
            case RType::GameState::GameWinFinish:
                diplayFinalScore();
                NextStageButton = generiqueComponent.CreateButton(world, "button_1", "button_1", "Credit", 14.75f, 60.f, false, 30.f,
                  [this] {
                    world.SetLevel(std::make_shared<creditLevel>(world));
                });
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

    void EndGameLevel::displayText(std::string text, std::string textureID, float x, float y) {
        std::shared_ptr<Entity> entity = world.GetRegistry().CreateEntity();
        world.GetRegistry().EmplaceComponent<TextComponent>(entity, text, textureID, x, y);
    }


    void EndGameLevel::diplayFinalScore() {
        displayText("You win all the Stage !", "Title", 50.f, 10.f);
        displayText("Your final score : " + std::to_string(world.score), "Title", 50.f, 50.f);
    }


    std::string_view EndGameLevel::GetName() const
    {
        return "end game level";
    }
} // MGE