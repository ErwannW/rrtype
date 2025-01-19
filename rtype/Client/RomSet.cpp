//
// Created by nailperreau on 25/11/24.
//

#include "RomSet.hpp"
#include "LobbyLevel.hpp"

#include "World.hpp"
#include "Entity.hpp"
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
        TextRoomSelect = world.GetRegistry().CreateEntity();

        world.GetRegistry().EmplaceComponent<SpriteComponent>(*BackgroundImage, "RoomDataBackGround", 100.f, 100.f);
        world.GetRegistry().EmplaceComponent<Common::TransformComponent>(*BackgroundImage, Common::TransformComponent(Vector3f(50.f, 50.f, 0)));

        world.GetRegistry().EmplaceComponent<TextComponent>(TextRoomSelect, "0", "Title_blue", 85.f, 52.5f);

        InputRoomName = generiqueComponent.CreateInput(world, "Input_1", "input_1", "My Room", 30, 27.f, 27.f, false, 78.f, 44.f,
         [this] {
         });

        ButtonPlayer1 = generiqueComponent.CreateButton(world, "buttonRoomData_1", "button_1", "", 66.f, 65.f, false, 7.f,
        [this] {
              nbMaxPlayersInNewRoom = 1;
              TextComponent& nbPlayer = world.GetRegistry().GetComponent<TextComponent>(*TextRoomSelect);
              nbPlayer.text = "1";
        });
        ButtonPlayer1_1 = generiqueComponent.CreateButton(world, "buttonRoomData_1", "button_1", "", 66.f, 65.f, false, 7.f,
        [this] {
              nbMaxPlayersInNewRoom = 1;
              TextComponent& nbPlayer = world.GetRegistry().GetComponent<TextComponent>(*TextRoomSelect);
              nbPlayer.text = "1";
        });
        ButtonPlayer2 = generiqueComponent.CreateButton(world, "buttonRoomData_2", "button_1", "", 74.f, 65.f, false, 7.f,
        [this] {
              nbMaxPlayersInNewRoom = 2;
              TextComponent& nbPlayer = world.GetRegistry().GetComponent<TextComponent>(*TextRoomSelect);
              nbPlayer.text = "2";
        });
        ButtonPlayer3 = generiqueComponent.CreateButton(world, "buttonRoomData_3", "button_1", "", 82.f, 65.f, false, 7.f,
        [this] {
              nbMaxPlayersInNewRoom = 3;
              TextComponent& nbPlayer = world.GetRegistry().GetComponent<TextComponent>(*TextRoomSelect);
              nbPlayer.text = "3";
        });
        ButtonPlayer4 = generiqueComponent.CreateButton(world, "buttonRoomData_4", "button_1", "", 90.f, 65.f, false, 7.f,
        [this] {
            nbMaxPlayersInNewRoom = 4;
            TextComponent& nbPlayer = world.GetRegistry().GetComponent<TextComponent>(*TextRoomSelect);
            nbPlayer.text = "4";
        });
        ButtonCreateRoom = generiqueComponent.CreateButton(world, "button_1", "button_1", "Create", 71.99f, 85.f, false, 20.f,
        [this] {
            if (CommandSendIsWaiting == false) {
                if (nbMaxPlayersInNewRoom == 0)
                  return;
                auto& textComponent = world.GetRegistry().GetComponent<TextComponent>(*InputRoomName);
                RoomData::SendCreateRoomDataRPC(textComponent.text, nbMaxPlayersInNewRoom, world);
                nbMaxPlayersInNewRoom = 0;
                textComponent.text = "My Room";
                auto& nbPlayer = world.GetRegistry().GetComponent<TextComponent>(*TextRoomSelect);
                nbPlayer.text = "0";
                CommandSendIsWaiting = true;
            }
        });
        ButtonExit = generiqueComponent.CreateButton(world, "button_1", "button_1", "Back", 14.75f, 90.f, false, 30.f,
        [this] {
            world.SetLevel(std::make_unique<LobbyLevel>(world));
        });
        world.engine.GetRoomData().SendRequestListDataRPC(world);
        CommandListIsWaiting = true;

        world.GetRegistry().EmplaceComponent<SongComponent>(*Song, "roomSet", true);

        for (int i = 0; i < 5; i++) {
            std::cout << "Entity " << i << std::endl;
            roomEntity entities;
            entities.room = world.GetRegistry().CreateEntity();
            world.GetRegistry().EmplaceComponent<SpriteComponent>(entities.room, SpriteComponent("RoomBackground", 50.f, 50.f));
            world.GetRegistry().EmplaceComponent<Common::TransformComponent>(entities.room, Common::TransformComponent(Vector3f(32.f, 30.f + 14 * i , 0)));
            world.GetRegistry().EmplaceComponent<TextComponent>(entities.room, TextComponent("", "Title_blue", 25.f, 30.f + 14 * i));
            world.GetRegistry().SetEntityEnabled(*entities.room, false);

            entities.button = world.GetRegistry().CreateEntity();
            entities.button = generiqueComponent.CreateButton(world, "JoinButton", "button_1", "", 52.f, 32.f + 14 * i, false, 6.f,
            [this] {});
            world.GetRegistry().SetEntityEnabled(*entities.button, false);

            entities.deletebutton = generiqueComponent.CreateButton(world, "DeleteButton", "button_1", "", 44.f, 32.f + 14 * i, false, 6.f,
            [this] {});
            world.GetRegistry().SetEntityEnabled(*entities.deletebutton, false);

            roomEntities.emplace_back(entities);
        }
    }

    int RomSet::newIndexForInfoBulle(int index) {
        for (int i = 0; i < infoBulletIndexe.size(); i++) {
            if (infoBulletIndexe[i] == index) {
                return newIndexForInfoBulle(index + 1);
            }
        }
        return index;
    }


    void RomSet::OnUpdate() {
        Level::OnUpdate();

        handleRoomDataReturn();
        handleRoomListUpdate();
        handleActionTimer();
    }

    void RomSet::handleActionTimer() {
        if (actionTimerIsWaiting == true) {
            deltaseconde += world.GetDeltaSeconds();
            if (deltaseconde >= 1.0f) {
                actionTimerIsWaiting = false;
                deltaseconde = 0.0f;
            }
        }
    }

    void RomSet::handleRoomDataReturn() {
        if (CommandSendIsWaiting) {
            while (world.engine.GetRoomData().GetReturnData().size() >= 1) {
                ReturnData returnValue = world.engine.GetRoomData().GetReturnData().front();
                processRoomDataReturn(returnValue);
                world.engine.GetRoomData().GetReturnData().pop();
                CommandSendIsWaiting = false;
            }
        }
    }

    void RomSet::processRoomDataReturn(const ReturnData& returnValue) {
        int index = newIndexForInfoBulle(0);
        infoBulletIndexe.push_back(index);

        if (returnValue.isSucces) {
            world.engine.GetLogger<>()->Info(" >> [NETWORK] >> [ROOM DATA] >> " + returnValue.message);
            RoomData::SendRequestListDataRPC(world);
            CommandListIsWaiting = true;

            addInfoBulleButton("infoBulleSucces", returnValue.message, index, 90.f);
        } else {
            world.engine.GetLogger<>()->Info(" >> [NETWORK] >> [ROOM DATA] >> " + returnValue.message);
            addInfoBulleButton("infoBulleError", returnValue.message, index, 90.f);
        }
    }

    void RomSet::addInfoBulleButton(const std::string& buttonType, const std::string& message, int index, float yPos) {
        infoBulle.push_back(generiqueComponent.CreateButton(world, buttonType, "Version", message, 90.f, yPos - infoBulle.size() * 17, false, 15.f,
            [this, index] {
                handleInfoBulleButtonClick(index);
            }));
    }

    void RomSet::handleInfoBulleButtonClick(int index) {
        int posEntity = 0;
        for (; posEntity < infoBulletIndexe.size(); posEntity++) {
            if (infoBulletIndexe[posEntity] == index) {
                break;
            }
        }

        world.GetRegistry().SetEntityEnabled(*infoBulle[posEntity], false);
        infoBulletIndexe.erase(infoBulletIndexe.begin() + posEntity);
        infoBulle.erase(infoBulle.begin() + posEntity);

        updateInfoBullePositions();
    }

    void RomSet::updateInfoBullePositions() {
        for (int i = 0; i < static_cast<int>(infoBulle.size()); i++) {
            auto &transformComponent = world.GetRegistry().GetComponent<Common::TransformComponent>(*infoBulle[i]);
            transformComponent.position.y = 90.f - i * 17;

            auto &texteComponent = world.GetRegistry().GetComponent<TextComponent>(*infoBulle[i]);
            texteComponent.position_y = 90.f - i * 17;
        }
    }

    void RomSet::handleRoomListUpdate() {
        //if (CommandListIsWaiting) {
            //if (world.engine.GetRoomData().GetRoomData().size() > 0 && world.engine.GetRoomData().receiveNewList == true) {
                world.engine.GetRoomData().receiveNewList = false;

                hideAllRoomEntities();

                for (int i = 0; i < world.engine.GetRoomData().GetRoomData().size(); i++) {
                    RoomDataStruct roomData = world.engine.GetRoomData().GetRoomData()[i];
                    displayRoomData(i, roomData);
                }

                CommandListIsWaiting = false;
            //}
        //}
    }

    void RomSet::hideAllRoomEntities() {
        for (int i = 0; i < 5; i++) {
            world.GetRegistry().SetEntityEnabled(*roomEntities[i].button, false);
            world.GetRegistry().SetEntityEnabled(*roomEntities[i].room, false);
            world.GetRegistry().SetEntityEnabled(*roomEntities[i].deletebutton, false);
        }
    }

    void RomSet::displayRoomData(int i, const RoomDataStruct& roomData) {
        world.GetRegistry().SetEntityEnabled(*roomEntities[i].room, true);
        auto &roomTextComponent = world.GetRegistry().GetComponent<TextComponent>(*roomEntities[i].room);
        roomTextComponent.text = roomData.roomName;

        bool isMyRoom = false;
        for (int i1 : roomData.playersEnroled) {
            if (i1 == world.client->GetClientId()) {
                isMyRoom = true;
            }
        }

        world.GetRegistry().SetEntityEnabled(*roomEntities[i].button, true);
        auto buttonButtonComponent = &world.GetRegistry().GetComponent<ButtonComponent>(*roomEntities[i].button);
        auto buttonSpriteComponent = &world.GetRegistry().GetComponent<SpriteComponent>(*roomEntities[i].button);

        if (!isMyRoom) {
            buttonSpriteComponent->texture_id = "JoinButton";
            buttonButtonComponent->onClick = [this, i] {
                if (CommandSendIsWaiting == false) {
                    if (actionTimerIsWaiting == false) {
                        world.engine.GetRoomData().SendJoinRequestDataRPC(this->world.engine.GetRoomData().GetRoomData()[i].roomID, this->world);
                        CommandSendIsWaiting = true;
                        actionTimerIsWaiting = true;

                    }
                }
            };
        } else {
            buttonSpriteComponent->texture_id = "LeaveButton";
            buttonButtonComponent->onClick = [this, i] {
                if (CommandSendIsWaiting == false) {
                    if (actionTimerIsWaiting == false) {
                        world.engine.GetRoomData().SendLeaveRequestDataRPC(this->world.engine.GetRoomData().GetRoomData()[i].roomID, this->world);
                        CommandSendIsWaiting = true;
                        actionTimerIsWaiting = true;
                    }
                }
            };
        }

        world.GetRegistry().SetEntityEnabled(*roomEntities[i].deletebutton, true);
        auto &deleteButtonButtonComponent = world.GetRegistry().GetComponent<ButtonComponent>(*roomEntities[i].deletebutton);

        deleteButtonButtonComponent.onClick = [this, i] {
            if (CommandSendIsWaiting == false) {
                world.engine.GetRoomData().SendDestroyRoomDataRPC(this->world.engine.GetRoomData().GetRoomData()[i].roomID, this->world);
                CommandSendIsWaiting = true;
            }
        };
    }

    void RomSet::OnClose() {
        world.engine.GetRenderer<SfmlDisplay>()->StopAllSong();
    }

    std::string_view RomSet::GetName() const
    {
        return "Room level";
    }
} // MGE