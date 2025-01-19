//
// Created by louiscroci on 26/11/24.
//

#ifndef SETSERVERADDR_HPP
#define SETSERVERADDR_HPP

// ENGINE INCLUDES
#include "../../Engine/Entity.hpp"
#include "../../Engine/Level/Level.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

namespace MGE
{
    class SetServerAddr : public Level
    {
        public:
            explicit SetServerAddr(World& world);
            void OnStart() override;

            [[nodiscard]] std::string_view GetName() const override;

        private:
            std::string serverAdresse;
            std::shared_ptr<MGE::Entity> BackgroundImage;
            std::shared_ptr<MGE::Entity> Song;
            std::shared_ptr<MGE::Entity> Input_SRV_ADDR;
            std::shared_ptr<MGE::Entity> ButtonValidations;
            std::string _serverAddr;
            GenericComponent generiqueComponent;

    };
} // MGE

#endif //SETSERVERADDR_HPP
