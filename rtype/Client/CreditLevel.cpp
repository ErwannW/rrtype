//
// Created by nailperreau on 25/11/24.
//

#include "CreditLevel.hpp"
#include "LobbyLevel.hpp"
#include "../Engine/Commons/Components/VelocityComponent.hpp"
#include "../Engine/Commons/Systems.hpp"

#include "World.hpp"
#include "Entity.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"

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

        world.GetRegistry().AddSystem(Common::VelocitySystem);

        world.GetRegistry().EmplaceComponent<SpriteComponent>(*BackgroundImage, "credit", 400.f, 400.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(*BackgroundImage, Common::TransformComponent(MGE::Vector3f(50.f, 300.f, 0)));
        world.GetRegistry().EmplaceComponent<Common::VelocityComponent>(*BackgroundImage, Common::VelocityComponent(MGE::Vector3f(0.f, -10.f, 0.f)));
        world.GetRegistry().EmplaceComponent<SongComponent>(*Song, "credit_song", true);


    }

    std::string_view creditLevel::GetName() const
    {
        return "Credit level";
    }

    void creditLevel::OnUpdate() {
        time += world.GetDeltaSeconds();
        if (time > 60.0f)
            exit(0);
    }
} // MGE