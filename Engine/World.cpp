//
// Created by Nail on 19/11/2024.
//

#include "World.hpp"
#include "Asserts.hpp"
#include "BuildTarget.hpp"
#include "Engine.hpp"
#include "Random.hpp"
#include "Registry.hpp"
#include "Types.hpp"
#include "Network/Packets/DestroyEntity.hpp"
#include "Network/Packets/EnableEntityPacket.h"
#include "Network/Packets/ReplicateProperty.hpp"
#include "Network/Packets/Rpc.hpp"
#include "Reflection/Class.hpp"
#include "Reflection/Rpc.hpp"
#include "../rtype/Network/GameNetwork.hpp"
#include "../rtype/component/SystemeComponent.hpp"

#include <cstdlib>

namespace MGE
{
    constexpr uint16 MinPort = 1023;
    constexpr uint16 MaxPort = std::numeric_limits<uint16>::max();

    World::World(Engine &engine) : engine(engine), registry(std::make_shared<Registry>(*this)), replicationUpdatesPerSeconds(20)
    {
        if constexpr (BuildTarget::IsServerBuild()) {
            server = std::make_unique<ABN::Server>();

            server->SetVerboseLevel(verbose_level::debug);
            server->SetVerbose(false);

            server->Open();
        } else if constexpr (MGE::BuildTarget::IsClientBuild()) {
            client = std::make_unique<ABN::Client>();

            client->SetVerboseLevel(verbose_level::debug);
            client->SetVerbose(false); // BuildTarget::CurrentBuildConfiguration == BuildTarget::BuildConfiguration::Debug
        }
    }

    Registry& World::GetRegistry()
    {
        return *registry;
    }

    const Registry& World::GetRegistry() const
    {
        return *registry;
    }

    void World::Update()
    {
        const auto end = std::chrono::system_clock::now();

        const std::chrono::duration<double, std::milli> duration = end - start;
        deltaSeconds = std::max(0., duration.count() / 1000);

        start = std::chrono::system_clock::now();

        if (currentLevel != nullptr)
            currentLevel->OnUpdate();

        registry->RunSystems();
        NetworkUpdate();

        ++currentFrame;
    }

    void World::NetworkUpdate()
    {
        CollectPackets();

        if constexpr (BuildTarget::IsServerBuild())
            SendReplicationUpdate();
    }

    void World::CollectPackets()
    {
        std::vector<ABN::packet_t> packets;

        if (server != nullptr)
            server->GetPackets(packets);
        else if (client != nullptr)
            client->GetPackets(packets);

        if (packets.empty())
            return;

        for (ABN::packet_t &packet: packets)
        {
            if (packet.header_.command_code_ == 0) {
                players.emplace_back(packet.header_.client_id_, 0);
                engine.GetLogger()->Info("Player #" + std::to_string(packet.header_.client_id_) + " joined the lobby.");
                continue;
            } else if (packet.header_.command_code_ == 1) {
                if (currentLevel != nullptr)
                    currentLevel->OnPlayerLeft(packet.header_.client_id_);

                std::erase_if(players, [playerId = packet.header_.client_id_](const Player& player) -> bool {
                    return player.playerId == playerId;
                });
                engine.GetLogger()->Info("Player #" + std::to_string(packet.header_.client_id_) + " left the lobby.");
                continue;
            }

            if (!CHECK(packet.header_.body_size_ >= sizeof(NetPacket), "Received an invalid packet of size " + std::to_string(packet.header_.body_size_)))
                continue;

            const auto* mgePacket = reinterpret_cast<const NetPacket *>(packet.body_.data());

            switch (mgePacket->commandId) {
            case NetCommand::ReplicateProperty:
                ReceivedPropertyReplication(static_cast<const ReplicateProperty *>(mgePacket));
                break;
            case NetCommand::Rpc:
                ReceivedRPC(static_cast<const RPCPacket *>(mgePacket));
                break;
            case NetCommand::Travel:
                break;
            case NetCommand::EnableEntity:
                ReceivedEnableEntityRPC(*static_cast<const EnableEntityPacket*>(mgePacket));
                break;
            case NetCommand::Game:
                RType::RtypeNetwork::GameNetworkReceiver(*engine.world, static_cast<const RType::GameNetwork*>(mgePacket), packet.header_.client_id_);
                break;
            case NetCommand::RoomGestion:
                engine.GetRoomData().ReceiveRoomDataRPC(static_cast<const RoomPacket::RoomGestionPacket*>(mgePacket), packet.header_.client_id_, *engine.world);
               break;
            default:
              CHECK(false, "Unknown command id '" + std::to_string(static_cast<int>(mgePacket->commandId)) + "'");
            }
        }
    }

    const std::vector<Player>& World::GetPlayers() const
    {
        return players;
    }

    std::vector<Player>& World::GetPlayers()
    {
        return players;
    }

    uint16 World::GetLocalPlayerId() const
    {
        return client != nullptr ? client->GetClientId() : 0;
    }

    void World::SendReplicationUpdate()
    {
        if constexpr (BuildTarget::IsClientBuild())
            return;

        secondsBeforeNextPropertyReplicationUpdate -= GetDeltaSeconds();

        if (secondsBeforeNextPropertyReplicationUpdate > 0)
            return;

        for (ObjectChangesTracker &obj: GetRegistry().replicator.GetTrackedObjects()) {
            for (const Property &changedProperty: obj.GetChangedProperties())
                SendPropertyReplication(obj.GetNetId(), obj.GetTarget(), changedProperty);
            obj.ResetSavedState();
        }

        secondsBeforeNextPropertyReplicationUpdate = 1 / replicationUpdatesPerSeconds;
    }

    World::FrameCountType World::GetCurrentFrame() const
    {
        return currentFrame;
    }

    double World::GetDeltaSeconds() const noexcept
    {
        return deltaSeconds;
    }

    float32 World::GetFDeltaSeconds() const noexcept
    {
        return static_cast<float32>(GetDeltaSeconds());
    }

    void World::SetLevel(const std::shared_ptr<ILevel> &level)
    {
        if (!VERIFY(level != nullptr, "Trying to load a null level."))
            return;

        if (currentLevel != nullptr) {
            engine.GetLogger()->Info("Closing level \"" + std::string(currentLevel->GetName()) + "\"");
            currentLevel->OnClose();
        }

        GetRegistry().DisableAllEntities();

        currentLevel = level;

        if (currentLevel != nullptr) {
            engine.GetLogger()->Info("Starting level \"" + std::string(currentLevel->GetName()) + '"');
            currentLevel->OnStart();
        }
    }

    NetId World::GetNewUniqueNetID() noexcept
    {
        return static_cast<NetId>(nextId++);
    }

    void World::CallRPC(const RPC &rpc, const Entity &entity, const std::size_t dataSize, const void *data) const
    {
        if (!VERIFY(entity.GetNetId() != NetId::None, "RPCs can only be called on replicated entities."))
            return;
        if (!VERIFY(rpc.paramSize == dataSize, "Incoherent RPC data size: input data differs from registered RPC param size (" + rpc.GetFullName() + ')'))
            return;

        const std::size_t headerSize = sizeof(RPCPacket);

        RPCPacket packetHeader;
        packetHeader.commandId = NetCommand::Rpc;
        packetHeader.targetEntity = entity.GetNetId();
        packetHeader.componentHash = rpc._class.GetHash();
        packetHeader.rpcHash = rpc.GetHash();

        using PacketHeaderType = std::remove_pointer_t<decltype(packetHeader)>;

        if (rpc.paramSize == 0) {
            client->Send(&packetHeader, sizeof(PacketHeaderType), 99, rpc.reliability == NetReliability::Reliable);
        } else {
            std::byte* packet = new std::byte[headerSize + dataSize];

            std::memcpy(packet, &packetHeader, headerSize);
            std::memcpy(packet + headerSize, data, dataSize);

            client->Send(packet, static_cast<uint16>(sizeof(PacketHeaderType) + dataSize), 99, rpc.reliability == NetReliability::Reliable);
            delete[] packet;
        }
    }

    void World::ReceivedRPC(const RPCPacket *packet)
    {
        if (packet == nullptr)
            return;

        std::shared_ptr<Entity> entity = registry->GetEntityByNetID(packet->targetEntity);

        if (!CHECK(entity != nullptr, "Received a RPC for an entity that doesn't exists."))
            return;

        std::optional<std::reference_wrapper<const Class>> _class = registry->GetRegisteredClassByHash(packet->componentHash);

        if (!CHECK(_class.has_value(), "Received a RPC for a component class that is not registered exists."))
            return;

        std::optional<std::reference_wrapper<const RPC>> rpc = _class.value().get().TryGetRPC(packet->rpcHash);

        if (!CHECK(rpc.has_value(), "Received a RPC that is not registered."))
            return;

        registry->ExecuteRPC(*rpc, *entity, reinterpret_cast<const std::byte *>(packet + 1));
    }


    bool World::JoinSession(const std::string &ip, const int16 tcpPort)
    {
        if (!VERIFY(client != nullptr, "Cannot join a session without an active client."))
            return false;

        client->Disconnect();

        srand(static_cast<unsigned int>(time(nullptr)));
        const uint16 localUdpPort = Random(rand()).Integer(MinPort, MaxPort);

        const uint16 udpPort = tcpPort + 1;

        const bool success = VERIFY(client->SetHost(ip)                  , "Failed to set host ip to " + ip)
                          && VERIFY(client->SetTcpPort(tcpPort)          , "Failed to set tcp port to " + std::to_string(tcpPort))
                          && VERIFY(client->SetUdpPort(udpPort)          , "Failed to set udp port to " + std::to_string(udpPort))
                          && VERIFY(client->Connect()                    , "Failed to connect to host " + ip + ':' + std::to_string(tcpPort));
                          //&& VERIFY(client->SetLocalUdpPort(localUdpPort), "Failed to set local udp port to " + std::to_string(localUdpPort))

        if (success)
            engine.GetLogger()->Info("Successfully connected to server " + ip + ':' + std::to_string(udpPort));

        return success;
    }

    void World::SendPropertyReplication(const NetId netId, void *obj, const Property &property)
    {
        if constexpr (!BuildTarget::IsServerBuild())
            return;

        if (obj == nullptr)
            return;

        ReplicateProperty packet;
        packet.commandId = NetCommand::ReplicateProperty;

        const auto& entity = GetRegistry().GetEntityByNetID(netId);

        EXPECT(entity != nullptr, "Entity with the given net id is null.");

        if (!CHECK(entity != nullptr, "Could not find the association entity of a component to replicate."))
            return;
        if (!CHECK(entity->isReplicated, "Trying to replicate a component of an entity that is not replicated."))
            return;

        packet.targetNetId = entity->GetNetId();
        packet.targetClass = property._class.GetHash();
        packet.targetProperty = property.GetHash();

        const std::size_t packetSize = sizeof(ReplicateProperty) + property.GetSize();

        if (!CHECK(packetSize <= static_cast<std::size_t>(std::numeric_limits<uint16>::max()), "Trying to send an invalid packet size of " + std::to_string(packetSize)))
            return;

        static Buffer propertyBuffer = Buffer(sizeof(std::byte) * 2048);

        if (propertyBuffer == nullptr)
            return;

        std::memcpy(propertyBuffer, &packet, sizeof(ReplicateProperty));
        std::memcpy(propertyBuffer + sizeof(ReplicateProperty), property.GetAddress(obj), property.GetSize());

        server->Send(propertyBuffer, static_cast<uint16>(packetSize), 99, false);
    }

    void World::Clients_DestroyEntity(const Entity &entity) const
    {
        if constexpr (!BuildTarget::IsServerBuild())
            return;

        DestroyEntityPacket packet;
        packet.entityNetId = entity.GetNetId();
        server->Send(&packet, sizeof(DestroyEntityPacket), 99, true);
    }

    void World::ReceivedRPCDestroyEntity(const DestroyEntityPacket &packet)
    {
        const std::shared_ptr<Entity> entity = GetRegistry().GetEntityByNetID(packet.entityNetId);

        if (entity == nullptr)
            return;
        GetRegistry().DestroyEntity(entity);
    }

    void World::SendEnableEntityRPC(const Entity& entity) const
    {
        EnableEntityPacket packet;

        packet.entityNetId = entity.GetNetId();
        packet.enabled = entity.IsEnabled();
        server->Send(&packet, sizeof(EnableEntityPacket), 99, true);
    }

    void World::ReceivedEnableEntityRPC(const EnableEntityPacket& packet)
    {
        std::shared_ptr<Entity> entity = GetRegistry().GetEntityByNetID(packet.entityNetId);
        if (!Entity::IsValid(entity)) {
            return;
        }

        GetRegistry().SetEntityEnabled(*entity, packet.enabled);
    }

    void World::ReceivedPropertyReplication(const ReplicateProperty *packet)
    {
        if (packet == nullptr)
            return;

        std::optional<std::reference_wrapper<const Class>> _class = registry->GetRegisteredClassByHash(packet->targetClass);
        if (!VERIFY(_class.has_value(), "Received an unknown class."))
            return;

        std::optional<std::reference_wrapper<const Property>> property = _class->get().TryGetProperty(packet->targetProperty);
        if (!VERIFY(property.has_value(), "Received an unknown property."))
            return;

        std::shared_ptr<Entity> entity = GetRegistry().GetEntityByNetID(packet->targetNetId);
        if (!VERIFY(entity != nullptr, "Received a net id that isn't bound to any entity (" + ToString(packet->targetNetId) + ')'))
            return;

        GetRegistry().SetPropertyValue(*entity, property.value().get(), reinterpret_cast<const std::byte *>(packet + 1));
    }

} // MGE
