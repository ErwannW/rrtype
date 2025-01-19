//
// Created by Nail on 18/11/2024.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstddef>
#include <memory>

#include "Types.hpp"
#include "Network/NetId.hpp"

namespace MGE
{
    class Registry;

    /**
     * Represents an entity of the ECS.
     */
    class Entity
    {
        public:
            using ComponentId = std::size_t;
            static ComponentId constexpr InvalidComponentId = -1;

            explicit Entity(Registry &registry);

            [[nodiscard]] uint64 GetId() const;
            [[nodiscard]] NetId GetNetId() const;
            void SetNetId(NetId id);


            [[nodiscard]] bool IsValid() const noexcept;

            [[nodiscard]] bool IsReplicated() const noexcept;

            void SetEnabled(bool enabled) noexcept;
            [[nodiscard]] bool IsEnabled() const noexcept;

            [[nodiscard]] bool IsDestroyed() const noexcept;

            static bool IsValid(const std::shared_ptr<Entity>& entity);
            static bool IsValid(const Entity& entity);

            static bool IsValidAndEnabled(const std::shared_ptr<Entity>& entity);
            static bool IsValidAndEnabled(const Entity& entity);

            bool isReplicated = false;

            ClientId owner = 0;
 
        private:
            bool enabled = true;
            Registry& registry;

            ComponentId componentId = InvalidComponentId;

            NetId netId = NetId::None;

            bool destroyed = false;
            uint64 id;
            inline static uint64 nextID = 0;

            friend class Registry;
            friend class std::shared_ptr<Entity>;
    };

    bool operator==(const Entity&a, const Entity& b);
}

#endif //ENTITY_HPP
