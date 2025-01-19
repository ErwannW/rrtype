//
// Created by nailperreau on 25/11/24.
//

// ENGINE INCLUDES
#include "../../Engine/World.hpp"

// LOCAL INCLUDES
#include "RomSet.hpp"
#include "LobbyLevel.hpp"


namespace MGE
{
    RomSet::RomSet(World &world) : Level(world)
    {
    }

    void RomSet::OnStart()
    {
        Level::OnStart();

        BackgroundImage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();

        world.GetRegistry().EmplaceComponent<SpriteComponent>(*BackgroundImage, "TekkenSpriteMenu", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(*BackgroundImage, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<SongComponent>(*Song, "TekkenSongMenu", true);
    }

    std::string_view RomSet::GetName() const
    {
        return "Credit level";
    }
} // MGE