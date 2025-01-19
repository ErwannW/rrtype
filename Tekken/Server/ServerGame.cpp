/*
** EPITECH PROJECT, 2024
** Engine
** File description:
** LevelOne
*/

#include <thread>
#include <chrono>
#include <string>
#include <algorithm>

// ENGINE INCLUDES
#include "../../Engine/World.hpp"
#include "../../Engine/Entity.hpp"
#include "../../Engine/Player.hpp"
#include "../../Engine/Commons/Components/Transform.hpp"

// LOCAL INCLUDES
#include "ServerGame.hpp"
#include "ServerLobby.hpp"
#include "../Systems/Systems.h"
#include "../Component/ComponentTekken.hpp"


namespace MGE::Server
{

    ServerGame::ServerGame(World &world) : Level(world), spawn() {}

    std::shared_ptr<Entity> ServerGame::CreatePlayerSpaceship(const ClientId ownerId) {
        auto entity = world.GetRegistry().CreateEntity();

        if (entity == nullptr)
            return entity;

        entity->isReplicated = true;
        entity->owner = ownerId;
        entity->SetNetId(world.GetNewUniqueNetID());
        playersEntities.emplace_back(entity);
        return entity;
    }


    void ServerGame::OnStart()
    {
        Level::OnStart();

        world.GetRegistry().AddSystem(Tekken::ProjectileSystemServer);
        world.GetRegistry().AddSystem(Tekken::SpaceshipSystem);
        world.GetRegistry().AddSystem(Tekken::EnnemySystem);
        world.GetRegistry().AddSystem(Tekken::AutoDestroySysteme);

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
                network.SendRPCServerOrderGameManagement(RType::GameState::GameOver, world);
                world.gameState = RType::GameState::GameOver;
                for (auto &player : players) {
                    player.gameEvent = RType::EventType::Lobby;
                    player.life = 3;
                }
                return;
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
                nbSpaceships++;
                network.SendRPCEntity(createPlayerEntityPacket, world);
            }
        } else if (isLevelActive() == false && gameStarted == true) {
            auto& players = world.GetPlayers();
            for (auto& player : players) {
                player.gameEvent = RType::EventType::Lobby;
                player.life = 3;
            }
            if (stageIsWin) {
                world.gameState = RType::GameState::GameWin;
                network.SendRPCServerOrderGameManagement(RType::GameState::GameWin, world);
            } else {
                world.gameState = RType::GameState::GameOver;
                network.SendRPCServerOrderGameManagement(RType::GameState::GameOver, world);
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

        constexpr std::array<const char*, 2> sprites = {
            "TekkenSpriteFighterRyu",
            "TekkenSpriteFighterKen",
        };

        std::size_t spriteIndex = 0;
        SendBackgroundToClient();
        for (const Player& playerInfo : world.GetPlayers()) {
            std::shared_ptr<Entity> player = CreatePlayerSpaceship(playerInfo.playerId);


            Vector3f pos;
            if (spriteIndex == 0)
                pos = Vector3f(10.f, 75.f, 0.f);
            else
                pos = Vector3f(80.f, 75.f, 0.f);

            world.GetRegistry().EmplaceComponent<Common::TransformComponent>(player, Common::TransformComponent(pos));
            world.GetRegistry().EmplaceComponent<SpaceshipComponent>(player, SpaceshipComponent(false, 1));

            const RType::ManageEntity createPlayerEntityPacket(pos, RType::EntityType::Player, HashString(sprites[spriteIndex++]), player->GetNetId(), player->owner);
            nbSpaceships++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            network.SendRPCEntity(createPlayerEntityPacket, world);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
