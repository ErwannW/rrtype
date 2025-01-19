//
// Created by Nail on 19/11/2024.
//

#ifndef WORLD_HPP
#define WORLD_HPP

#include <chrono>
#include <memory>
#include "Player.hpp"
#include "Level/ILevel.hpp"
#include "display/sfml/SfmlDisplay.hpp"
#include "Network/NetId.hpp"
#include "Network/Packets/DestroyEntity.hpp"
#include "Network/Packets/EnableEntityPacket.h"
#include "Network/Packets/Rpc.hpp"
#include "Network/Packets/ReplicateProperty.hpp"

namespace MGE
{
    class Registry;

    class World
    {
        public:
            World(const World&) = delete;
            virtual ~World() = default;
            std::string EngineVersion = "0.1"; /**< Engine version (compatible if the first digit matches with Client and Server) */
            std::string ClientVersion = "0.1"; /**< Engine version (compatible if the first digit matches with Engine and Server) */
            std::string ServerVersion = "0.1"; /**< Engine version (compatible if the first digit matches with Client and Engine) */
            using FrameCountType = std::uint32_t;

            /**
             * A world cannot exist without an engine.
             */
            explicit World(class Engine& engine);

            /**
             * This world's engine.
             */
            Engine& engine;

            /**
             * @return The active registry of this world.
             */
            Registry &GetRegistry();

            /**
             * @return The active registry of this world.
             */
            [[nodiscard]] const Registry &GetRegistry() const;

            /**
             * Updates the world.
             * Will run all systems once.
             */
            void Update();

            void NetworkUpdate();
            void CollectPackets();
            void SendReplicationUpdate();

            NetId GetNewUniqueNetID() noexcept;

            /**
             * GetValue the current frame of this world.
             * @remark This is not the global engine's frame counter.
             * @return The current frame of this world.
             */
            [[nodiscard]] FrameCountType GetCurrentFrame() const;

            [[nodiscard]] double GetDeltaSeconds() const noexcept;

            [[nodiscard]] float32 GetFDeltaSeconds() const noexcept;

            void SetLevel(const std::shared_ptr<ILevel> &level);

            template<typename TParam>
            void CallRPC(const RPC& rpc, const Entity& entity, const TParam& param);

            void SendPropertyReplication(NetId netId, void *obj, const Property& property);
            void Clients_DestroyEntity(const Entity &entity) const;
            void ReceivedRPCDestroyEntity(const DestroyEntityPacket &packet);
            void SendEnableEntityRPC(const Entity& entity) const;

            void ReceivedRPC(const RPCPacket *packet);
            void ReceivedPropertyReplication(const ReplicateProperty *packet);
            void ReceivedEnableEntityRPC(const EnableEntityPacket& packet);

            std::unique_ptr<ABN::Server> server;
            std::unique_ptr<ABN::Client> client;

            bool JoinSession(const std::string& ip, int16 tcpPort);

            const std::vector<Player> &GetPlayers() const;
            std::vector<Player> &GetPlayers();


            [[nodiscard]] uint16 GetLocalPlayerId() const;

            //variable à mettre dans GameLevel.hpp ultèrieurement
            RType::GameState gameState = RType::GameState::Loading;

            //ServerLobby.hpp et je doit y avoir acces dans LevelOne.hpp
            std::vector<std::string> luaFileLevelNames;
            int currentGameLevel = 0;
            int howPlayerContinueToPlay = 0;
            mutable int CurrentLife = 3;
            mutable int score = 0;

        private:
            std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
             /**
             * This world's registry.
             */
            std::shared_ptr<Registry> registry;

            /**
             * This world's frame counter. It is increased each update.
             */
            FrameCountType currentFrame = 0;

            double deltaSeconds = 0;

            std::shared_ptr<ILevel> currentLevel;

            std::underlying_type_t<NetId> nextId = 1;

            std::vector<Player> players;
            double worldLifetime = 0;

            void CallRPC(const RPC &rpc, const Entity &entity, std::size_t dataSize, const void *data) const;

            double secondsBeforeNextPropertyReplicationUpdate = 0;
            double replicationUpdatesPerSeconds = 50;
    };

    template<typename TParam>
    void World::CallRPC(const RPC &rpc, const Entity& entity, const TParam &param)
    {
        return CallRPC(rpc, entity, sizeof(TParam), std::addressof(param));
    }
} // MGE

#endif //WORLD_HPP
