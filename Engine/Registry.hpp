//
// Created by Nail on 18/11/2024.
//

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <any>
#include <functional>
#include <memory>
#include "Entity.hpp"
#include "Containers/SparseArray.hpp"
#include "World.hpp"
#include "Hashing/Hash.hpp"
#include "Reflection/Property.hpp"
#include "Reflection/Class.hpp"
#include "Containers/Dictionary.hpp"
#include "Network/ObjectReplicator.hpp"

#include <BuildTarget.hpp>

namespace MGE
{
    class System;

    struct EntityRef
    {
        REFLECT_STRUCT(EntityRef)

        std::shared_ptr<Entity> entity;
    };

    /**
     * Manages the entities of a world.
     */
    class Registry
    {
        public:
            explicit Registry(const Registry&) = delete;
            /**
             * A registry cannot exist without a world.
             */
            explicit Registry(class World& world);

            Registry& operator=(const Registry&) = delete;

            World& world;

            /**
             * Creates a new entity.
             * @return A new entity.
             */
            std::shared_ptr<Entity> CreateEntity();

            /**
             * Destroys an entity and all of its components.
             * @param entity The entity to destroy.
             */
            void DestroyEntity(const std::shared_ptr<Entity> &entity);

            /**
             * Emplaces a new component on the given entity.
             * @tparam TComponent The type of the component to add.
             * @tparam TParams The type of the arguments passed to the constructor of TComponent.
             * @param entity The entity on which tp add the component.
             * @param p The params that will be passed to the constructor of the components.
             * @return
             */
            template<typename TComponent, typename... TParams>
            typename SparseArray<TComponent>::reference_type EmplaceComponent(const std::shared_ptr<Entity>& entity, TParams &&... p);

            /**
             * Emplaces a new component on the given entity.
             * @tparam TComponent The type of the component to add.
             * @tparam TParams The type of the arguments passed to the constructor of TComponent.
             * @param entity The entity on which tp add the component.
             * @param p The params that will be passed to the constructor of the components.
             * @return
             */
            template<typename TComponent, typename... TParams>
            typename SparseArray<TComponent>::reference_type EmplaceComponent(const Entity& entity, TParams &&... p);

            /**
             * Removes a component of the given entity.
             * @tparam TComponent The type fo the component to remove.
             * @param entity The entity of the component that is getting removed from.
             * @see RemoveComponents
             */
            template<typename TComponent>
            void RemoveComponent(const std::shared_ptr<Entity>& entity);

            /**
             * Removes multiple components at once.
             * @tparam TComponents The types of the components to remove.
             * @param entity The entity of the components that are getting removed from.
             * @see RemoveComponent
             */
            template<typename... TComponents>
            void RemoveComponents(const std::shared_ptr<Entity>& entity);

            /**
             * Registers a new component type.
             * @note AddComponent/Emplace components will automatically register components types.
             * @tparam TComponent The type of the component to register.
             * @return The array of this component type.
             */
            template<typename TComponent>
            SparseArray<TComponent>& RegisterComponentType();

            /**
             * GetValue all components of this type.
             * @tparam TComponent The type of the components to get.
             * @return The internal array of this component type.
             */
            template<typename TComponent>
            SparseArray<TComponent>& GetComponents();

            /**
             * GetValue all components of this type.
             * @tparam TComponent The type of the components to get.
             * @return The internal array of this component type.
             */
            template<typename TComponent>
            const SparseArray<TComponent>& GetComponents() const;

            /**
             * Returns the Component of the entity.
             * @tparam TComponent The Component to get.
             * @param entity the entity the Component will be fetched from.
             * @return The address of the entity component will be returned.
             */
            template<typename TComponent>
            [[nodiscard]] TComponent& GetComponent(const Entity& entity);

            template<typename TComponent>
            [[nodiscard]] const TComponent& GetComponent(const Entity& entity) const;

            /**
             * Returns the Component of the entity.
             * @tparam TComponent The Component to get.
             * @param entity the entity the Component will be fetched from.
             * @return The address of the entity component will be returned.
             */
            template<typename TComponent>
            [[nodiscard]] TComponent* GetComponentAddress(const Entity& entity);

            /**
             * Check if an entity have specific Components.
             * @tparam TComponents Single or Multiples Components to be checked.
             * @param entity Entity that will be checked to see if it owns the wanted Component(s).
             * @return True if the entity owns all Components, false otherwise.
             */
            template<typename... TComponents>
            [[nodiscard]] bool HasComponent(const Entity& entity);

            void SetEntityEnabled(Entity& entity, bool enabled);

            /**
             * Adds a new system.
             */
            void AddSystem(const std::shared_ptr<System>& system);
            /**
             * Removes a new system.
             */
            void RemoveSystem(const std::shared_ptr<System>& system);

            /**
             * Checks if this system has been registered.
             * @param system The system to look for.
             * @return true if the system was found, false otherwise.
             */
            [[nodiscard]] bool HasSystem(const std::shared_ptr<System>& system) const;

            /**
             * Runs all the systems once.
             */
            void RunSystems();

            /**
             * Destroys all entities of the registry.
             */
            void DestroyAllEntities();
            void DisableAllEntities();

            void SetPropertyValue(Entity &entity, const Property &property, const std::byte* value);

            ObjectReplicator replicator;

            [[nodiscard]] std::shared_ptr<Entity> GetEntityByComponentId(std::size_t componentId) const;
            [[nodiscard]] std::shared_ptr<Entity> GetEntityByNetID(NetId netId) const;

            template<typename TComponent>
            [[nodiscard]] std::shared_ptr<Entity> GetEntityByComponent(const TComponent& component);

            [[nodiscard]] std::shared_ptr<Entity> GetEntityByVoidComponent(const void* component);

            [[nodiscard]] std::optional<std::reference_wrapper<const Class>> GetRegisteredClassByHash(Hash componentClassHash) const;

            [[nodiscard]] const std::vector<std::shared_ptr<Entity>>& GetEntities() const;

            void ExecuteRPC(const RPC& rpc, const Entity& entity, const std::byte *data);

        private:
            template<typename TComponent>
            SparseArray<TComponent> &GetOrCreateComponentArray();

            std::size_t nextComponentId = 0;
            [[nodiscard]] std::size_t GetNewComponentId();

            using ApplyPropFuncPrototype = void(Registry&, const Entity&, const Property&, const std::byte*);
            using ExecuteRPCFuncPrototype = void(Registry&, const Entity&, const RPC&, const std::byte*);

            std::vector<std::size_t> availableComponentsIds;
            std::vector<std::shared_ptr<Entity>> entities;
            Dictionary<std::reference_wrapper<const Class>, std::any> componentsArray;
            Dictionary<std::reference_wrapper<const Class>, std::function<void(Registry&, const Entity&)>> resetOptionalFunc;
            Dictionary<std::reference_wrapper<const Class>, std::function<void(Registry&, const Entity&)>> startReplicationTracking;
            Dictionary<std::reference_wrapper<const Class>, std::function<void(Registry&, const Entity&)>> stopReplicationTracking;
            Dictionary<std::reference_wrapper<const Class>, std::function<ApplyPropFuncPrototype>> applyPropValueFunc;
            Dictionary<std::reference_wrapper<const Class>, std::function<ExecuteRPCFuncPrototype>> executeRPCFunc;
            Dictionary<std::reference_wrapper<const Class>, std::function<std::size_t(Registry&, const void*)>> getComponentId;
            std::vector<std::shared_ptr<System>> systems;
    };

    template<typename TComponent, typename... TParams>
    typename SparseArray<TComponent>::reference_type Registry::EmplaceComponent(const std::shared_ptr<Entity>& entity, TParams&&... p)
    {
        EXPECT(Entity::IsValid(entity), "Cannot add a component to an invalid entity.");
        return EmplaceComponent<TComponent>(*entity, std::forward<TParams>(p)...);
    }

    template<typename TComponent, typename ... TParams>
    typename SparseArray<TComponent>::reference_type Registry::EmplaceComponent(const Entity &entity, TParams &&...p)
    {
        EXPECT(Entity::IsValid(entity), "Cannot add a component to an invalid entity.");

        static_assert(std::is_constructible_v<TComponent, TParams...>, "TComponent must be constructible from TParams.");

        auto &array = GetOrCreateComponentArray<TComponent>();
        auto& component = array.emplace_at(entity.componentId, std::forward<TParams>(p)...);

        if constexpr (BuildTarget::IsServerBuild()) {
            if (component.has_value() && entity.IsReplicated() && TComponent::GetClass().ShouldReplicate())
              replicator.TrackObject(TComponent::GetClass(), std::addressof(component.value()), entity.GetNetId());
        }

        return component;
    }

    template<typename TComponent>
    void Registry::RemoveComponent(const std::shared_ptr<Entity> &entity)
    {
        if (!resetOptionalFunc.Contains(TComponent::GetClass()))
            return;

        resetOptionalFunc.at(TComponent::GetClass())(*this, entity);
    }

    template<typename ... TComponents>
    void Registry::RemoveComponents(const std::shared_ptr<Entity> &entity)
    {
        (..., RemoveComponent<TComponents>(entity));
    }

    template<typename TComponent>
    SparseArray<TComponent>& Registry::RegisterComponentType()
    {
        const Class& componentClass = TComponent::GetClass();

        if (componentsArray.Contains(componentClass))
            return std::any_cast<SparseArray<TComponent>&>(componentsArray.at(componentClass));

        resetOptionalFunc.Emplace(componentClass, [](Registry &registry, const Entity& entity) -> void {
            if (!Entity::IsValid(entity))
                return;

            auto &comps = registry.GetComponents<TComponent>();
            auto &comp = comps[entity.componentId];

            if (comp.has_value())
                registry.replicator.StopTracking(std::addressof(comp.value()));

            comp.reset();
        });

        applyPropValueFunc.Emplace(componentClass, [](Registry &registry, const Entity& entity, const Property& property, const std::byte* value) -> void {
            auto& opt = registry.GetComponents<TComponent>()[entity.componentId];

            if (!opt.has_value())
                return;

            property.SetValue(std::addressof(opt.value()), value);
        });

        executeRPCFunc.Emplace(componentClass, [](Registry& registry, const Entity& entity, const RPC& rpc, const std::byte* param) -> void {
            auto& opt = registry.GetComponents<TComponent>()[entity.componentId];

            if (!opt.has_value())
                return;

            rpc.Call(std::addressof(opt.value()), param);
        });

        getComponentId.Emplace(componentClass, [](Registry& registry, const void* param) -> std::size_t {
            SparseArray<TComponent>& array = registry.GetComponents<TComponent>();

            for (std::size_t i = 0; i < array.size(); ++i) {
                if (!array[i].has_value())
                    continue;

                const void* addr = std::addressof(array[i].value());
                if (param == addr)
                    return i;
            }
            return -1;
        });

        startReplicationTracking.Emplace(componentClass, [](Registry& registry, const Entity& entity) -> void {
            if (!VERIFY(entity.isReplicated, "Cannot start replication tracking on an entity not marked as replicated."))
                return;

            SparseArray<TComponent>& array = registry.GetComponents<TComponent>();
            registry.replicator.TrackObject(TComponent::GetClass(), registry.GetComponentAddress<TComponent>(entity), entity.GetNetId());
        });

        stopReplicationTracking.Emplace(componentClass, [](Registry& registry, const Entity& entity) -> void {
            SparseArray<TComponent>& array = registry.GetComponents<TComponent>();

            if (array[entity.componentId].has_value())
                registry.replicator.StopTracking(registry.GetComponentAddress<TComponent>(entity));
        });

        world.engine.GetLogger()->Info("Registered component \"" + componentClass.GetName() + '"');

        return std::any_cast<SparseArray<TComponent>&>(componentsArray[componentClass] = SparseArray<TComponent>());
    }

    template<typename TComponent>
    SparseArray<TComponent>& Registry::GetComponents()
    {
        return GetOrCreateComponentArray<TComponent>();
    }

    template<typename TComponent>
    const SparseArray<TComponent>& Registry::GetComponents() const
    {
        return std::any_cast<SparseArray<TComponent>&>(componentsArray.at(TComponent::GetClass()));
    }

    template<typename... TComponents>
    bool Registry::HasComponent(const Entity& entity)
    {
        return (... && GetComponents<TComponents>()[entity.componentId].has_value());
    }

    template<typename TComponent>
    const TComponent& Registry::GetComponent(const Entity& entity) const
    {
        return *GetComponents<TComponent>()[entity.componentId];
    }

    template <typename TComponent>
    TComponent* Registry::GetComponentAddress(const Entity &entity)
    {
        auto& comp = GetComponents<TComponent>()[entity.componentId];

        if (!comp.has_value())
            return nullptr;
        return std::addressof(comp.value());
    }

    template<typename TComponent>
    TComponent & Registry::GetComponent(const Entity &entity)
    {
        return *GetComponents<TComponent>()[entity.componentId];
    }

    template<typename... TComponents>
    auto GetSize(Registry &r)
    {
        return (..., r.GetComponents<TComponents>().size());
    }

    template<typename TComponent>
    std::shared_ptr<Entity> Registry::GetEntityByComponent(const TComponent &component)
    {
        const void* addr = std::addressof(component);
        auto id = getComponentId.at(TComponent::GetClass())(*this, addr);

        if (id == -1)
            return nullptr;

        auto it = std::ranges::find_if(entities, [id](auto &entity) -> bool {
            return entity->componentId == id;
        });

        if (it != entities.end())
            return *it;
        return nullptr;
    }

    template<typename TComponent>
    SparseArray<TComponent> &Registry::GetOrCreateComponentArray()
    {
        if (!componentsArray.Contains(TComponent::GetClass()))
            RegisterComponentType<TComponent>();

        return std::any_cast<SparseArray<TComponent>&>(componentsArray.at(TComponent::GetClass()));
    }
} // MGE

#endif //REGISTRY_HPP
