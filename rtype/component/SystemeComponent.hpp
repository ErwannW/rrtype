/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** component-r-type
*/

#ifndef COMPONENT_R_TYPE_HPP_
#define COMPONENT_R_TYPE_HPP_

#include <string>
#include <queue>
#include <utility>
#include <memory>
#include "../../Engine/Level/ILevel.hpp"
#include <functional>
#include "../../Engine/Entity.hpp"
#include "../../Engine/Engine.hpp"
#include "Core.hpp"
#include "../../Engine/Commons/Components/Transform.hpp"
#include "../server/ServerGame.hpp"

struct LifeComponent {
    REFLECT_STRUCT(LifeComponent)

    int lifeNumber = 0;

    explicit LifeComponent(int lifeNumber);
};

struct ScoreComponent {
    REFLECT_STRUCT(ScoreComponent)

    explicit ScoreComponent();
};

struct AutoDestroyComponent {
    REFLECT_STRUCT(AutoDestroyComponent)

    float32 elapsedTime = 0.0f;
    float32 lifeTime = 0.0f;
    bool sprite = false;
    bool song = false;
    explicit AutoDestroyComponent(float32 lifeTime, bool sprite, bool song);
};

struct ProjectileAnimationsComponent {
    REFLECT_STRUCT(ProjectileAnimationsComponent)

    float32 elapsedTime = 0.0f;
    float32 lifeTime = 0.0f;
    std::string NewTextureID;
    explicit ProjectileAnimationsComponent(float32 lifeTime, std::string);
};

struct SpaceshipComponent {
    REFLECT_STRUCT(SpaceshipComponent)

    float32 speed = 0.0f;
    bool isMyShip = false;

    float32 lastShoot = 0.0f;
    float32 shootCooldown = 0.0f;

    explicit SpaceshipComponent(bool isMyShip, float speed);
};

struct ProjectileComponent {
    REFLECT_STRUCT(ProjectileComponent)

    int32 orientation;
    int32 type;     // 0 for ally, 1 for enemy
    int32 id;       // id for the correct sprite

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

struct EnnemyShooterComponent {
    REFLECT_STRUCT(EnnemyShooterComponent)

    std::string EnemyType;
    std::string ProjectileType;
    double lastShot = 0;
    MGE::Server::EnemyConfig &config;


    explicit EnnemyShooterComponent(std::string EnemyType, std::string ProjectileType, MGE::Server::EnemyConfig &config);
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

        static void AutoDestroySpriteSysteme(MGE::World &world, MGE::EntityRef& entity, AutoDestroyComponent &autoDestroyComponent, SpriteComponent &spriteComponent);
        static void AutoDestroySongSysteme(MGE::World &world, MGE::EntityRef& entity, AutoDestroyComponent &autoDestroyComponent, SongComponent &songComponent);

        static void SendDegat(float positionX, float positionY);
        static void DeleteEntityOnClient(MGE::World &world, MGE::NetId netId);
        static void LifeSystem(MGE::World &world, MGE::EntityRef &entity, LifeComponent &lifeComponent);
        static void ScoreSysteme(MGE::World &world, MGE::EntityRef &entity, ScoreComponent &scoreComponent, TextComponent &textComponent);
        static void ProjectileAnimationsSystemServer(MGE::World &world, ProjectileAnimationsComponent &projectileAnimationsComponent, SpriteComponent &spriteComponent);
        static void EnnemyShooterSysteme(MGE::World &world, EnnemyShooterComponent &ennemyShooterComponent, MGE::Common::TransformComponent &transformComponent);

        RType::RtypeNetwork network;
};

#endif /* !COMPONENT_R_TYPE_HPP_ */