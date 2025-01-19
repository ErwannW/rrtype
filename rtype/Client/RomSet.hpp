//
// Created by louiscroci on 26/11/24.
//

#ifndef ROMSET_HPP
#define ROMSET_HPP

#include "Entity.hpp"
#include "Level/Level.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"

namespace MGE
{
    struct roomEntity {
        std::shared_ptr<Entity> room;
        std::shared_ptr<Entity> button;
        std::shared_ptr<Entity> deletebutton;
    };

    class RomSet : public Level
    {
        public:
            explicit RomSet(World& world);
            void OnStart() override;
            void OnUpdate() override;
            void OnClose() override;

            void handleActionTimer();

            void handleRoomDataReturn();

            void processRoomDataReturn(const ReturnData &returnValue);

            int newIndexForInfoBulle(int index);

            void addInfoBulleButton(const std::string &buttonType, const std::string &message, int index, float yPos);

            void handleInfoBulleButtonClick(int index);

            void updateInfoBullePositions();

            void handleRoomListUpdate();

            void hideAllRoomEntities();

            void displayRoomData(int i, const RoomDataStruct &roomData);

            [[nodiscard]] std::string_view GetName() const override;

        private:
            int nbMaxPlayersInNewRoom;

            std::shared_ptr<Entity> BackgroundImage;
            std::shared_ptr<Entity> Song;
            
            std::shared_ptr<Entity> TextRoomSelect;
            std::shared_ptr<Entity> InputRoomName;

            std::shared_ptr<Entity> ButtonPlayer1;
            std::shared_ptr<Entity> ButtonPlayer1_1;
            std::shared_ptr<Entity> ButtonPlayer2;
            std::shared_ptr<Entity> ButtonPlayer3;
            std::shared_ptr<Entity> ButtonPlayer4;
            std::shared_ptr<Entity> ButtonCreateRoom;
            std::shared_ptr<Entity> ButtonExit;

            double deltaseconde = 0.0;
            bool actionTimerIsWaiting = false;
            bool CommandListIsWaiting = false;
            bool CommandSendIsWaiting = false;
            GenericComponent generiqueComponent;
            bool buttonIsWaiting = false;
            double buttonIsWaitingTime = 0;
            std::vector<roomEntity> roomEntities;
            std::vector<std::shared_ptr<Entity>> infoBulle;
            std::vector<int> infoBulletIndexe;
    };
} // MGE

#endif //ROMSET_HPP
