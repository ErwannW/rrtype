/*
** EPITECH PROJECT, 2024
** Engine
** File description:
** LevelOne
*/

#include "ServerGame.hpp"
#include "../../Engine/Commons/Components/Transform.hpp"
#include "../../Engine/World.hpp"
#include "../Client/LobbyLevel.hpp"
#include "../rtype/Systems/Systems.h"
#include "../rtype/component/SystemeComponent.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "ServerLobby.hpp"
#include <algorithm>
#include <chrono>
#include <string>
#include <thread>

namespace MGE::Server
{

    ServerGame::ServerGame(World &world) : Level(world), spawn() {
    }

    std::shared_ptr<Entity> ServerGame::CreatePlayerSpaceship(const ClientId ownerId) {
        auto entity = world.GetRegistry().CreateEntity();

        entity->isReplicated = true;
        entity->owner = ownerId;
        entity->SetNetId(world.GetNewUniqueNetID());
        playersEntities.emplace_back(entity);
        return entity;
    }

    void ServerGame::OnStart()
    {
        Level::OnStart();

        world.GetRegistry().AddSystem(RType::ProjectileSystemServer);
        world.GetRegistry().AddSystem(RType::SpaceshipSystem);
        world.GetRegistry().AddSystem(RType::EnnemySystem);
        world.GetRegistry().AddSystem(RType::EnnemyShooterSysteme);
        luaL_openlibs(StageLuaScript);
        if (luaL_dofile(StageLuaScript, "LevelConfiguration/GameConf.lua")) {
            world.engine.GetLogger()->Error("When we load GameConf");
            exit(500);
            lua_pop(StageLuaScript, 1);
        }
        loadAllLevel();
        if (world.luaFileLevelNames.empty()) {
            world.engine.GetLogger()->Error("No level found in the configuration file");
            return;
        }
        if (world.currentGameLevel >= world.luaFileLevelNames.size()) {
            world.engine.GetLogger()->Error("The level number is not correct");
            return;
        }
        if (world.GetPlayers().size() == world.howPlayerContinueToPlay) {
          world.currentGameLevel ++;
          world.howPlayerContinueToPlay = 0;
        }
        if (world.currentGameLevel >= world.luaFileLevelNames.size()) {
            for (const std::vector<Player> &players = world.GetPlayers(); auto & player : players) {
            RType::RtypeNetwork::SendRPCServerOrderGameManagement(RType::GameState::Error, world);
            world.engine.GetLogger()->Error("The level number is not correct");
            world.SetLevel(std::make_shared<ServerLobby>(world));
            world.currentGameLevel = 0;
            return;
          }
        }
        std::string levelFile = "LevelConfiguration/" + world.luaFileLevelNames[world.currentGameLevel] + ".lua";
        loadScript(levelFile);
    }

    void ServerGame::OnUpdate() {
        Level::OnUpdate();

        if (gameStarted == false && std::ranges::all_of(world.GetPlayers(), [](const Player &player) -> bool {
            return player.gameEvent == RType::EventType::InGame;
        })) {
            network.SendRPCServerOrderGameManagement(RType::GameState::InGame, world);
            std::this_thread::sleep_for(std::chrono::duration<float>(std::chrono::seconds(1)));
            gameStarted = true;
            LaunchGame();
        } else if (gameStarted == true && isLevelActive() == true) {
            auto players = world.GetPlayers();
            const bool areAllPlayersDead = std::ranges::all_of(players, [](const Player& p) -> bool {
                return p.IsDead();
            });
            if (areAllPlayersDead) {
                auto &players1 = world.GetPlayers();
                for (auto &player : players1) {
                    player.gameEvent = RType::EventType::Lobby;
                    player.life = 3;
                }
                world.gameState = RType::GameState::GameOver;
                std::future<void> result = std::async(std::launch::async, [&]() {
                    network.SendRPCServerOrderGameManagement(RType::GameState::GameOver, world);
                });
                result.get();
                return world.SetLevel(std::make_shared<ServerLobby>(world));
            }

            if (callUpdateLevel(world.GetDeltaSeconds(), spawn)) {
                std::shared_ptr<Entity> enemy = world.GetRegistry().CreateEntity();
                enemy->isReplicated = true;
                enemy->owner = playersEntities[0]->owner;
                enemy->SetNetId(world.GetNewUniqueNetID());
                world.GetRegistry().EmplaceComponent<Common::TransformComponent>(enemy, Common::TransformComponent(Vector3f(static_cast<float>(spawn.x), static_cast<float>(spawn.y), 0.f)));
                world.GetRegistry().EmplaceComponent<EnnemyComponent>(enemy, EnnemyComponent(allEnemyConf[spawn.type].speed, allEnemyConf[spawn.type].Point_of_live, allEnemyConf[spawn.type].isBosse, allEnemyConf[spawn.type].movingUp, allEnemyConf[spawn.type], spawn.movement));
                RType::ManageEntity createPlayerEntityPacket(Vector3f(static_cast<float>(spawn.x) , static_cast<float>(spawn.y), 0.f),
                RType::EntityType::Enemy, HashString(spawn.type), enemy->GetNetId(), 0);
                std::future<void> result = std::async(std::launch::async, [&]() {
                    RType::RtypeNetwork::SendRPCEntity(createPlayerEntityPacket, world);
                });
                result.get();
                if (spawn.projectile != "none") {
                    world.GetRegistry().EmplaceComponent<EnnemyShooterComponent>(enemy, EnnemyShooterComponent(spawn.type, spawn.projectile, allEnemyConf[spawn.projectile]));
                }
             }
        } else if (isLevelActive() == false && gameStarted == true) {
            auto &players = world.GetPlayers();
            for (auto &player : players) {
                player.gameEvent = RType::EventType::Lobby;
                player.life = 3;
            }
            if (stageIsWin) {
                world.gameState = RType::GameState::GameWin;
                if (world.luaFileLevelNames.size() == world.currentGameLevel + 1) {
                    std::future<void> result = std::async(std::launch::async, [&]() {
                        RType::RtypeNetwork::SendRPCServerOrderGameManagement(RType::GameState::GameWinFinish, world);
                    });
                    result.get();
                } else {
                    std::future<void> result = std::async(std::launch::async, [&]() {
                        RType::RtypeNetwork::SendRPCServerOrderGameManagement(RType::GameState::GameWin, world);
                    });
                    result.get();
                }
            } else {
                world.gameState = RType::GameState::GameOver;
                std::future<void> result = std::async(std::launch::async, [&]() {
                    RType::RtypeNetwork::SendRPCServerOrderGameManagement(RType::GameState::GameOver, world);
                });
                result.get();
            }
            world.SetLevel(std::make_shared<ServerLobby>(world));
        }
    }

    bool ServerGame::isLevelActive() const {
        lua_getglobal(StageLuaScript, "isLevelActive");
        if (lua_pcall(StageLuaScript, 0, 1, 0) != LUA_OK) {
            world.engine.GetLogger()->Error("When we call isLevelActive");
            return false;
        }

        bool result = lua_toboolean(StageLuaScript, -1);
        lua_pop(StageLuaScript, 1);
        return result;
    }

    void ServerGame::OnPlayerLeft(const ClientId playerId)
    {
        Level::OnPlayerLeft(playerId);

        for (const std::shared_ptr<Entity>& players_entity: playersEntities)
            if (players_entity->owner == playerId)
                world.GetRegistry().DestroyEntity(players_entity);
    }

    void ServerGame::CreateBackground(int id, float scroll, const std::string& texture_id)
    {
        std::shared_ptr<Entity> background = world.GetRegistry().CreateEntity();
        background->SetNetId(world.GetNewUniqueNetID());
        background->isReplicated = true;
        background->owner = 0;

        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(background, Common::TransformComponent(Vector3f((id == 1) ? 90.0f : 275.0f, 50.0f, 0.0f)));
        world.GetRegistry().EmplaceComponent<BackgroundComponent>(background, BackgroundComponent(scroll));
        RType::ManageEntity createBgEntityPacket(Vector3f((id == 1) ? 90.0f : 275.0f, 50.0f, 0.0f), RType::EntityType::UI,
          HashString(texture_id), background->GetNetId(), background->owner);
        network.SendRPCEntity(createBgEntityPacket, world);
    }

    void ServerGame::launchSongOnClient() const {
        std::shared_ptr<Entity> sound = world.GetRegistry().CreateEntity();
        sound->SetNetId(world.GetNewUniqueNetID());
        sound->isReplicated = false;
        sound->owner = 0;

        RType::ManageEntity createSongEntityPacket(Vector3f(0, 0, 0), RType::EntityType::LevelSond,
        HashString(levelMusicID), sound->GetNetId(), sound->owner);
        network.SendRPCEntity(createSongEntityPacket, world);
    }

    void ServerGame::LaunchGame()
    {
        world.engine.GetLogger()->Info("All players have joined, launching the game...");

        constexpr std::array<const char*, 4> sprites = {
            "spaceship_blue",
            "spaceship_green",
            "spaceship_red",
            "spaceship_pink",
        };

        std::size_t spriteIndex = 0;

        SendBackgroundToClient();
        for (const Player& playerInfo : world.GetPlayers()) {
            std::shared_ptr<Entity> player = CreatePlayerSpaceship(playerInfo.playerId);

            world.GetRegistry().EmplaceComponent<Common::TransformComponent>(player, Common::TransformComponent(Vector3f(20, 10, 0)));
            world.GetRegistry().EmplaceComponent<SpaceshipComponent>(player, SpaceshipComponent(false, 1));

            RType::ManageEntity createPlayerEntityPacket(Vector3f(20.f, 20.f * static_cast<float>(nbSpaceships) * 15, 0.f), RType::EntityType::Player,
              HashString(sprites[spriteIndex++]), player->GetNetId(), player->owner);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            nbSpaceships++;
            network.SendRPCEntity(createPlayerEntityPacket, world);
        }
        for (int i = 0; i < 3; i ++) {
            RType::ManageEntity createLifeEntityPacket(Vector3f(5.f + i * 120 , 10.f, 0.f), RType::EntityType::Life,
              HashString(std::to_string(i + 1)), world.GetNewUniqueNetID(), 0);
            std::future<void> result = std::async(std::launch::async, [&]() {
                network.SendRPCEntity(createLifeEntityPacket, world);
            });
            result.get();
        }
        gameStarted = true;
        launchSongOnClient();

    }

    void ServerGame::OnClose() {
        Level::OnClose();
        lua_close(StageLuaScript);
    }


    std::string_view ServerGame::GetName() const
    {
        return "Level One";
    }
}
// MGE