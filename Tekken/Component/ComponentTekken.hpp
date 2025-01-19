/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** component-r-type
*/

#ifndef COMPONENT_R_TYPE_HPP_
#define COMPONENT_R_TYPE_HPP_

#include <functional>

// ENGINE INCLUDES
#include "../../Engine/Core.hpp"
#include "../../Engine/Level/ILevel.hpp"
#include "../../Engine/Commons/Components/Transform.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

// LOCAL INCLUDES
#include "../Server/ServerGame.hpp"
#include "../Network/GameNetwork.hpp"
#include "../Network/GameNetworkEnum.hpp"

struct AutoDestroyComponent {
    REFLECT_STRUCT(AutoDestroyComponent)

    float32 elapsedTime = 0.0f;
    float32 lifeTime = 0.0f;

    explicit AutoDestroyComponent(float32 lifeTime);
};

struct AttackComponent {
    REFLECT_STRUCT(AttackComponent)

    bool isAttacking = false;
    float32 attackProgress = 0.0f;
    float32 attackCooldown = 0.0f;
    int32 id;

    explicit AttackComponent(bool isAttacking, float attackProgress, float attackCooldown, int id);
};

struct SpaceshipComponent {
    REFLECT_STRUCT(SpaceshipComponent)

    float32 speed = 0.0f;
    bool isMyShip = false;

    float32 lastShoot = 0.0f;
    float32 shootCooldown = 0.0f;

    bool isJumping = false;
    float32 jumpProgress = 0.0f;

    explicit SpaceshipComponent(bool isMyShip, float speed);
};

struct ProjectileComponent {
    REFLECT_STRUCT(ProjectileComponent)

    int32 orientation;
    int32 type;
    int32 id;

    explicit ProjectileComponent(int orientation, int type, int id);
};

struct EnnemyComponent {
    REFLECT_STRUCT(EnnemyComponent)

    float32 speed;
    int32 hp;
    bool isBoss;
    bool movingUp;
    std::string movement;
    MGE::Server::EnemyConfig &config;

    explicit EnnemyComponent(float speed, int hp, bool isBoss, bool movingUp, MGE::Server::EnemyConfig &config, std::string movement);
};

struct BackgroundComponent {
    REFLECT_STRUCT(BackgroundComponent)

    float32 scroll_speed = 0;
    bool is_paralax = false;

    explicit BackgroundComponent(float scroll_speed);
};

class ComponentRType {
    public:
        static void BackgroundSystem(MGE::World &world, BackgroundComponent &backgroundComponent, MGE::Common::TransformComponent &transformComponent);
        static void SpaceShipSystem(MGE::World &world, MGE::EntityRef& entity, SpaceshipComponent &spaceshipComponent, MGE::Common::TransformComponent &transformComponent);
        static void ProjectileSystemServer(MGE::World &world, MGE::EntityRef& entity, ProjectileComponent &projectileComponent, MGE::Common::TransformComponent &transformComponent);
        static void EnnemySystem(MGE::World &world, MGE::EntityRef& entity, EnnemyComponent &ennemyComponent, MGE::Common::TransformComponent &transformComponent);
        static bool CollisionCircle(MGE::Vector3f positionProjectile, MGE::Vector3f positionEnnemy, float size);
        static void AutoDestroySysteme(MGE::World &world, MGE::EntityRef& entity, AutoDestroyComponent &autoDestroyComponent);
        static void SendDegat(float positionX, float positionY);
        static void DeleteEntityOnClient(MGE::World &world, MGE::NetId netId);
        static void AttackSystem(MGE::World& world, MGE::EntityRef& entity, AttackComponent& attackComponent, MGE::Common::TransformComponent& transformComponent);
        RType::RtypeNetwork network;
};

#endif /* !COMPONENT_R_TYPE_HPP_ */
