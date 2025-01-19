/*
** EPITECH PROJECT, 2024
** r-type-2
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Types.hpp"
#include "../rtype/Network/GameNetworkEnum.hpp"

struct Player {
    Player() : playerId(0), score(0) {}
    Player(uint16 playerId, uint16 score) : playerId(playerId), score(score) {}
    std::string playerName;
    uint16 playerId = 0;
    uint16 score = 0;
    uint16 life = 3;
    RType::EventType gameEvent = RType::EventType::Lobby;

    constexpr bool IsDead() const
    {
        return life <= 0;
    }
};

#endif /* !PLAYER_HPP_ */
