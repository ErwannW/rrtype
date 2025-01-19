//
// Created by nailperreau on 25/11/24.
//

// ENGINE INCLUDES
#include ".././Engine/World.hpp"
#include ".././Engine/GeneriqueEntity/GeneriqueComponent.hpp"

// LOCAL INCLUDES
#include "LobbyLevel.hpp"
#include "CreditLevel.hpp"

namespace MGE
{
    creditLevel::creditLevel(World &world) : Level(world)
    {
    }

    void creditLevel::OnStart()
    {
        Level::OnStart();

        BackgroundImage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();

        world.GetRegistry().EmplaceComponent<SpriteComponent>(*BackgroundImage, "TekkenSpriteMenu", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(*BackgroundImage, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<SongComponent>(*Song, "TekkenSongMenu", true);
    }

    std::string_view creditLevel::GetName() const
    {
        return "Credit level";
    }
} // MGE