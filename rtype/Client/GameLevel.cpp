//
// Created by nailperreau on 25/11/24.
//

#include "GameLevel.hpp"

#include "World.hpp"
#include "LoadingLevel.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"
#include "../component/SystemeComponent.hpp"
#include "System/System.hpp"
#include "../Systems/Systems.h"
#include "EndGameLevel.hpp"

namespace MGE
{
    GameLevel::GameLevel(World &world) : Level(world)
    {
    }

    void GameLevel::OnStart()
    {
        Level::OnStart();
        world.GetRegistry().AddSystem(RType::BackgroundSystem);
        world.GetRegistry().AddSystem(RType::LifeSysteme);
        world.GetRegistry().AddSystem(RType::ScoreSysteme);
        world.GetRegistry().AddSystem(RType::AutoDestroySongSysteme);
        world.GetRegistry().AddSystem(RType::AutoDestroySpriteSysteme);
        world.GetRegistry().AddSystem(RType::ProjectileAnimationsSystemServer);
    }

    void GameLevel::OnUpdate() {
        switch (world.gameState) {
            case RType::GameState::GameWin:
                world.SetLevel(std::make_shared<EndGameLevel>(world));
                break;
            case RType::GameState::GameOver:
                world.SetLevel(std::make_shared<EndGameLevel>(world));
                break;
            case RType::GameState::Error:
                world.SetLevel(std::make_shared<EndGameLevel>(world));
                break;
            case RType::GameState::Loading:
                world.SetLevel(std::make_shared<LoadingLevel>(world));
                break;
            case RType::GameState::GameWinFinish:
                world.SetLevel(std::make_shared<EndGameLevel>(world));
                break;
            default:
                break;
        }
    }

    void GameLevel::OnClose()
    {
        world.engine.GetRenderer<SfmlDisplay>()->StopAllSong();
        world.CurrentLife = 3;
    }

    std::string_view GameLevel::GetName() const
    {
        return "Game Level";
    }
} // MGE