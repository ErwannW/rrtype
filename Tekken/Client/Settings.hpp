//
// Created by louiscroci on 26/11/24.
//

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

// ENGINE INCLUDES
#include "../../Engine/Entity.hpp"
#include "../../Engine/Level/Level.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

namespace MGE
{
    class Settings : public Level
    {
        public:
            explicit Settings(World& world);
            void OnStart() override;

            [[nodiscard]] std::string_view GetName() const override;

        private:
            std::shared_ptr<MGE::Entity> BackgroundImage;
            std::shared_ptr<MGE::Entity> Background;
            std::shared_ptr<MGE::Entity> Song;
            std::shared_ptr<MGE::Entity> eyeButton;
            std::shared_ptr<MGE::Entity> Pop_up;
            std::shared_ptr<MGE::Entity> ButtonRetour;
            std::shared_ptr<MGE::Entity> ButtonSave;

            std::shared_ptr<MGE::Entity> playerNameText;
            std::shared_ptr<MGE::Entity> playerNameInput;
            std::shared_ptr<MGE::Entity> playerNameButtonSave;

            std::shared_ptr<MGE::Entity> songLevelLogo;
            std::shared_ptr<MGE::Entity> songLevelText;
            std::shared_ptr<MGE::Entity> songLevelTextValue;
            std::shared_ptr<MGE::Entity> songLevelButtonMore;
            std::shared_ptr<MGE::Entity> songLevelButtonLess;

            std::shared_ptr<MGE::Entity> ServerVersionText;
            std::shared_ptr<MGE::Entity> ClientVersionText;
            std::shared_ptr<MGE::Entity> EngineVersionText;
            std::shared_ptr<MGE::Entity> accessibilityModeBouton;

            GenericComponent generiqueComponent;
    };
} // MGE

#endif //SETTINGS_HPP
