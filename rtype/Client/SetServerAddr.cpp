//
// Created by nailperreau on 25/11/24.
//

#include "SetServerAddr.hpp"
#include "LobbyLevel.hpp"

#include "../../Engine/World.hpp"
#include "../../Engine/Entity.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

namespace MGE
{
    SetServerAddr::SetServerAddr(World &world) : Level(world)
    {
    }

    void SetServerAddr::OnStart()
    {
        Level::OnStart();

        BackgroundImage = world.GetRegistry().CreateEntity();
        Song = world.GetRegistry().CreateEntity();

        world.GetRegistry().EmplaceComponent<SpriteComponent>(*BackgroundImage, "Loading_Backgroung", 115.f, 115.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(*BackgroundImage, MGE::Common::TransformComponent(MGE::Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<SongComponent>(*Song, "Loading_Song", true);

        Input_SRV_ADDR = generiqueComponent.CreateInput(world, "Input_1", "button_1", "127.0.0.1:8080", 30, 50.f, 50.f, false, 50.f, 50.f,
        [this]() {
        });
        ButtonValidations = generiqueComponent.CreateButton(world, "button_1", "button_1", "Validations", 14.75f, 80.f, false, 30.f,
        [this]() {
            TextComponent& textComponent = world.GetRegistry().GetComponent<TextComponent>(*Input_SRV_ADDR);
            size_t colonPos = textComponent.text.find(':');
            std::string ip = textComponent.text.substr(0, colonPos);
            const int16 port = std::stoi(textComponent.text.substr(colonPos + 1));
            if (world.JoinSession(ip, port))
                world.SetLevel(std::make_unique<LobbyLevel>(world));
        });
    }

    void SetServerAddr::OnClose()
    {

    }


    std::string_view SetServerAddr::GetName() const
    {
        return "SetValue Server Address";
    }
} // MGE