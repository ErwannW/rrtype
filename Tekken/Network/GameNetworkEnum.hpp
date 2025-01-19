//
// Created by louis on 13/01/2025.
//

#ifndef GAMENETWORKENUM_HPP
#define GAMENETWORKENUM_HPP

#include <string>

// ENGINE INCLUDES
#include "../../Engine/Types.hpp"

namespace RType {
    /**
    * @brief Type of entity.
    */
    enum class EntityType : uint8_t
    {
        Player,
        Enemy,
        Bullet,
        PowerUp,
        Explosion,
        LevelSond,
        Delete,
        UI
    };

    /**
    * @brief Event type for the spaceship.
    */
    enum class SpaceShipEventType : uint8
    {
        MoveDown,
        MoveUp,
        MoveLeft,
        MoveRight,
        Shot,
        Jump,
        Attack,
    };

    /**
    * @brief Type of event.
    */
    enum class EventType : uint8
    {
        Lobby,
        NextStage,
        InGame,
    };

    enum class GameState : uint8_t
    {
        Loading,
        InGame,
        GameOver,
        GameWin,
        Error,
    };

    struct BackgroundConfig {
        std::string spriteId;
        float speed;
    };

    enum class RtypeNetCommande {
        ManageEntity,
        DestroyEntity,
        SpaceShipEvent,
        GameEvent,
        ServerOrderGameManagement,
        Paralax,
    };
}

#endif //GAMENETWORKENUM_HPP
