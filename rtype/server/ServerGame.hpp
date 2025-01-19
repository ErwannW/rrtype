//
// Created by louiscroci on 26/11/24.
//

#ifndef LevelOne_HPP
#define LevelOne_HPP

#include <memory>
#include <vector>
#include <map>
#include "Level/Level.hpp"
#include "Entity.hpp"
#include "../Network/GameNetwork.hpp"

extern "C" {
    #include <lua.h>
    #include "lauxlib.h"
    #include "lualib.h"
}
namespace MGE
{
    namespace Server
    {
        struct SpawnInfo {
            std::string type;
            std::string movement;
            std::string projectile;
            int x;
            int y;
        };

        enum class HeatBoxType {
            Circle,
            Square,
        };

        struct EnemyConfig {
            int Point_of_live;
            float speed;
            std::string sprite_id;
            std::string destruction_id;
            int heatBoxSize;
            HeatBoxType heatBoxType;
            bool isBosse;
            bool movingUp;
        };

        struct ProjectileConfig {
            int damage;
            std::string sprite_id;
        };

        class ServerGame : public Level
        {
            public:
                explicit ServerGame(World& world);
                void OnStart() override;
                void OnUpdate() override;
                void OnClose() override;

                void OnPlayerLeft(ClientId playerId) override;

                [[nodiscard]] std::string_view GetName() const override;

                [[nodiscard]] std::shared_ptr<Entity> CreatePlayerSpaceship(ClientId ownerId);
                void CreateBackground(int id, float speed, const std::string& texture_id);
                [[nodiscard]] bool callUpdateLevel(double deltaTime, SpawnInfo& spawn);
                [[nodiscard]] bool extractSpawnInfo(SpawnInfo& spawn);
                void loadEnemyEntityFromConfig();
                void loadProjectileEntityFromConfig();
                void loadAllLevel() const;
                [[nodiscard]] bool isLevelActive() const;
                void loadScript(const std::string& filename);
                void loadBackground();
                void launchSongOnClient() const;
                void launchBackgroundOnCLient();
                void SendBackgroundToClient() const;
                void CreateProjectileForEnemy(std::string projectile) const;

            private:
                std::map<std::string, EnemyConfig> allEnemyConf;
                std::map<std::string, ProjectileConfig> allProjectilesConf;
                std::vector<RType::BackgroundConfig> paralaxConfig;
                std::string levelMusicID;
                bool gameStarted = false;
                std::vector<std::shared_ptr<Entity>> playersEntities;

                int nbSpaceships = 0;
                void LaunchGame();
                lua_State* StageLuaScript = luaL_newstate();
                lua_State* StageLuaConfig = luaL_newstate();


                SpawnInfo spawn;
                bool stageIsWin = true;
                RType::RtypeNetwork network;
        };
    }
} // MGE

#endif //LevelOne_HPP
