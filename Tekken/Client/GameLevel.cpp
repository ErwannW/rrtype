//
// Created by nailperreau on 25/11/24.
//

// ENGINE INCLUDES
#include "../../Engine/World.hpp"

// LOCAL INCLUDES
#include "GameLevel.hpp"
#include "LoadingLevel.hpp"
#include "EndGameLevel.hpp"
#include "../Systems/Systems.h"
#include "../Component/ComponentTekken.hpp"

namespace MGE
{
    GameLevel::GameLevel(World &world) : Level(world) {}

    void GameLevel::OnStart()
    {
        Level::OnStart();
        world.GetRegistry().AddSystem(Tekken::BackgroundSystem);

        Song = world.GetRegistry().CreateEntity();
        world.GetRegistry().EmplaceComponent<SongComponent>(Song, "TekkenSongStage", true);
    }

    void GameLevel::OnUpdate() {
        switch (world.gameState) {
            case RType::GameState::GameWin:
            case RType::GameState::GameOver:
            case RType::GameState::Error:
                world.SetLevel(std::make_shared<EndGameLevel>(world));
                break;
            case RType::GameState::Loading:
                world.SetLevel(std::make_shared<LoadingLevel>(world));
                break;
              default:
                break;
        }
    }

    void GameLevel::OnClose()
{
        world.engine.GetRenderer<SfmlDisplay>()->StopAllSong();
    }


    std::string_view GameLevel::GetName() const
    {
        return "Game Level";
    }
} // MGE