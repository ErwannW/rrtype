//
// Created by Nail on 18/11/2024.
//

#include "Entity.hpp"

namespace MGE
{
    Entity::Entity(Registry &registry) : registry(registry), id(nextID++){ }

    bool Entity::IsValid() const noexcept
    {
        return !destroyed;
    }

    NetId Entity::GetNetId() const
    {
        return netId;
    }

    uint64 Entity::GetId() const
    {
        return id;
    }

    bool Entity::IsReplicated() const noexcept
    {
        return isReplicated;
    }

    void Entity::SetEnabled(const bool enabled) noexcept
    {
        this->enabled = enabled;
    }

    bool Entity::IsEnabled() const noexcept
    {
        return enabled;
    }

    bool Entity::IsDestroyed() const noexcept
    {
        return destroyed;
    }

    bool Entity::IsValid(const std::shared_ptr<Entity> &entity)
    {
        return entity != nullptr && IsValid(*entity);
    }

    bool Entity::IsValid(const Entity &entity)
    {
        return entity.IsValid();
    }

    bool Entity::IsValidAndEnabled(const std::shared_ptr<Entity> &entity)
    {
        return entity != nullptr && IsValidAndEnabled(*entity);
    }

    bool Entity::IsValidAndEnabled(const Entity &entity)
    {
        return IsValid(entity) && entity.IsEnabled();
    }

    bool operator==(const Entity &a, const Entity &b)
    {
        return std::addressof(a) == std::addressof(b);
    }

    void Entity::SetNetId(NetId id)
    {
        netId = id;
    }
}
