//
// Created by Nail on 18/11/2024.
//

#include <ranges>
#include "Registry.hpp"
#include "Asserts.hpp"
#include "World.hpp"
#include "Commons/Components/Transform.hpp"
#include "System/System.hpp"

#include <BuildTarget.hpp>

namespace MGE
{
    void EntityRef::SetupReflection()
    {

    }

    Registry::Registry(World &world) : world(world)
    {

    }

    const std::vector<std::shared_ptr<Entity>>& Registry::GetEntities() const
    {
        return entities;
    }

    std::shared_ptr<Entity> Registry::CreateEntity()
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(*this);

        entity->componentId = GetNewComponentId();

        entities.emplace_back(entity);

        for (const auto &func: resetOptionalFunc | std::views::values)
            func(*this, *entity);

        EmplaceComponent<EntityRef>(entity, entity);
        return entity;
    }

    void Registry::DestroyEntity(const std::shared_ptr<Entity> &entity)
    {
        return SetEntityEnabled(*entity, false);
        if (!Entity::IsValid(entity))
            return;

        for (const auto &func : resetOptionalFunc | std::views::values)
            func(*this, *entity);

        if constexpr (BuildTarget::IsServerBuild())
        {
            if (entity->isReplicated)
                world.Clients_DestroyEntity(*entity);

            for (const auto &func : stopReplicationTracking | std::views::values)
                func(*this, *entity);
        }

        entity->destroyed = true;
        availableComponentsIds.emplace_back(entity->componentId);
        entity->componentId = Entity::InvalidComponentId;

        std::erase_if(entities, [&entity](const std::shared_ptr<Entity>& entity2) -> bool {
            return entity == entity2;
        });
    }

    void Registry::SetEntityEnabled(Entity& entity, const bool enabled)
    {
        if (entity.enabled == enabled)
            return;

        entity.SetEnabled(enabled);
        if constexpr (BuildTarget::CurrentBuildType == BuildTarget::BuildType::Server)
        {
            if (entity.isReplicated)
                world.SendEnableEntityRPC(entity);

            if (entity.IsEnabled()) {

            } else {

            }
        }
    }

    void Registry::AddSystem(const std::shared_ptr<System> &system)
    {
        if (!CHECK(system != nullptr, "Trying to add a null system."))
            return;

        if (HasSystem(system))
            return world.engine.GetLogger()->Warning("System \"" + std::string(system->GetName()) + "\" already exists.");

        const std::size_t numberOfSystems = systems.size();

        systems.emplace_back(system);

        std::ranges::sort(systems, [](const std::shared_ptr<System> &lhs, const std::shared_ptr<System> &rhs) -> bool {
            return lhs->order > rhs->order;
        });

        VERIFY(systems.size() == numberOfSystems + 1, "Incoherent number of system: " + std::to_string(numberOfSystems)  + " before adding a new one, " + std::to_string(systems.size()) + " now.");

        if (systems.size() <= 1)
            world.engine.GetLogger()->Info("Added system \"" + std::string(system->GetName()) + "\", " + std::to_string(systems.size()) + " system registered in total.");
        else
            world.engine.GetLogger()->Info("Added system \"" + std::string(system->GetName()) + "\", " + std::to_string(systems.size()) + " systems registered in total.");
    }

    void Registry::RemoveSystem(const std::shared_ptr<System> &system)
    {
        if (!CHECK(system != nullptr, "Trying to remove a null system."))
            return;

        systems.erase(std::ranges::find(systems, system));
        world.engine.GetLogger()->Info("Removed system \"" + std::string(system->GetName()) + '"');
    }

    bool Registry::HasSystem(const std::shared_ptr<System> &system) const
    {
        return std::ranges::find(systems, system) != systems.end();
    }

    void Registry::RunSystems()
    {
        for (const std::shared_ptr<System>& system: systems)
            (*system)(*this);
    }

    void Registry::DestroyAllEntities()
    {
        world.engine.GetLogger()->Info("Destroying all (" + std::to_string(entities.size()) + ") entities.");

        while (!entities.empty())
            DestroyEntity(entities.back());
    }

    void Registry::DisableAllEntities()
    {
        for (auto& entity : entities) {
            if (Entity::IsValidAndEnabled(entity))
                SetEntityEnabled(*entity, false);
        }
    }

    void Registry::SetPropertyValue(Entity &entity, const Property &property, const std::byte* value)
    {
        if (!CHECK(Entity::IsValid(entity), "Trying to set the property of an invalid entity."))
            return;

        applyPropValueFunc.at(property._class)(*this, entity, property, value);
    }

    std::size_t Registry::GetNewComponentId()
    {
        return nextComponentId++;
        if (availableComponentsIds.empty())
            return nextComponentId++;

        const auto newId = availableComponentsIds.back();
        availableComponentsIds.pop_back();
        return newId;
    }

std::shared_ptr<Entity> Registry::GetEntityByComponentId(const std::size_t componentId) const
{
    for (const std::shared_ptr<Entity> &entity: entities)
        if (entity->componentId == componentId)
            return entity;

    return nullptr;
}

    std::shared_ptr<Entity> Registry::GetEntityByNetID(const NetId netId) const
    {
        for (const std::shared_ptr<Entity> &entity: entities)
            if (entity->netId == netId)
                return entity;

        return nullptr;
    }

    std::shared_ptr<Entity> Registry::GetEntityByVoidComponent(const void *component)
    {
        for (const auto &findFunc: getComponentId | std::views::values) {
            auto id = findFunc(*this, component);

            auto it = std::ranges::find_if(entities, [id](auto &entity) -> bool {
                return entity->componentId == id;
            });

            if (it != entities.end())
                return *it;
        }
        return nullptr;
    }

    std::optional<std::reference_wrapper<const Class>> Registry::GetRegisteredClassByHash(MGE::Hash componentClassHash) const
    {
        for (const Class &_class: componentsArray | std::views::keys)
            if (componentClassHash == _class.GetHash())
                return _class;

        return std::nullopt;
    }

    void Registry::ExecuteRPC(const RPC &rpc, const Entity &entity, const std::byte *data)
    {
        std::optional<std::reference_wrapper<const Class>> _class = GetRegisteredClassByHash(rpc._class.GetHash());

        if (!CHECK(_class.has_value(), "Received a RPC for an component class that is not registered."))
            return;

        executeRPCFunc.at(*_class)(*this, entity, rpc, data);
    }
} // MGE