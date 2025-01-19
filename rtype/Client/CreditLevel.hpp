//
// Created by louiscroci on 26/11/24.
//

#ifndef CREDITLEVEL_HPP
#define CREDITLEVEL_HPP

#include "Entity.hpp"
#include "Level/Level.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"

namespace MGE
{
    class creditLevel : public Level
    {
        public:
            explicit creditLevel(World& world);
            void OnStart() override;
            void OnUpdate() override;

            [[nodiscard]] std::string_view GetName() const override;
            double time = 0.0;
        private:
            std::shared_ptr<Entity> BackgroundImage;
            std::shared_ptr<Entity> Song;
            
            std::shared_ptr<Entity> GameNameText;
            std::shared_ptr<Entity> GameDisigneTeam;
            std::shared_ptr<Entity> GameDisigneTeamCredit;
            std::shared_ptr<Entity> GameEngineTeam;
            std::shared_ptr<Entity> GameEngineTeamCredit;
            std::shared_ptr<Entity> ResauxTeam;
            std::shared_ptr<Entity> ResauxTeamCredit;
            std::shared_ptr<Entity> RenderTeam;
            std::shared_ptr<Entity> RenderTeamCredit;


            GenericComponent generiqueComponent;
    };
} // MGE

#endif //CREDITLEVEL_HPP
