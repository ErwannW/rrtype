cmake_minimum_required(VERSION 3.5)
project(Rtype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPM_URL "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake")
set(CPM_PATH "${CMAKE_BINARY_DIR}/CPM.cmake")

message(STATUS "Downloading CPM from ${CPM_URL} to ${CPM_PATH}...")
file (DOWNLOAD
        ${CPM_URL}
        ${CPM_PATH}
        SHOW_PROGRESS
)

include(${CPM_PATH})

if (WIN32)
    add_definitions(-DWINDOWS)
    add_definitions(-DWINVER=0x0A00)
    add_definitions(-D_WIN32_WINNT=0x0A00)
elseif (UNIX)
    add_definitions(-DLINUX)
elseif (APPLE)
    add_definitions(-DMACOSX)
else()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is not a supported platform.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_BUILD)
    message(STATUS "Building in Debug mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE_BUILD)
    message(STATUS "Building in Release mode")
else()
    message(FATAL "Unknown build type.")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/Zc:preprocessor)
    add_compile_options(/MP)
endif()

include(FetchContent)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

CPMAddPackage(NAME lua
        GITHUB_REPOSITORY nperreau/lua-cmake
        VERSION 1.0.2)
set(LUA_INCLUDE_DIR ${lua_SOURCE_DIR})

CPMAddPackage(NAME asio
        GITHUB_REPOSITORY chriskohlhoff/asio
        GIT_TAG asio-1-32-0)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)

CPMAddPackage(NAME SFML
        GITHUB_REPOSITORY SFML/SFML
        VERSION 2.6.2
        GIT_TAG 2.6.2
        OPTIONS
        "SFML_GENERATE_PACKAGE=FALSE"
        "SFML_STATIC_LIBRARIES=TRUE")

set(RTYPE_SOURCES
        ../Engine/Engine.hpp
        ../Engine/Library/DynamicLibrary.hpp
        ../Engine/Containers/SparseArray.hpp
        ../Engine/Registry.hpp
        ../Engine/Entity.hpp
        ../Engine/System/Query.hpp
        ../Engine/World.hpp
        ../Engine/NetRoles.hpp
        ../Engine/Bitflag.hpp
        ../Engine/Rendering/IRenderer.hpp
        ../Engine/Logging/ILogger.hpp
        ../Engine/Logging/DefaultLogger.hpp
        ../Engine/Asserts.hpp
        ../Engine/Exceptions/Exception.hpp
        ../Engine/Module/IModule.hpp
        ../Engine/Module/ModuleInfo.hpp
        ../Engine/System/System.hpp
        ../Engine/Level/ILevel.hpp
        ../Engine/GeneriqueEntity/GeneriqueComponent.hpp
        ../Engine/display/sfml/SfmlDisplay.hpp
        ../Engine/display/IDisplay.hpp
        ../Engine/display/Lua/Lua.hpp
        ../Engine/Stacktrace.hpp
        ../Engine/Level/Level.hpp
        ../Engine/EngineObject.hpp
        ../Engine/Network/NetId.hpp
        ../Engine/Network/Packets/NetPacket.hpp
        ../Engine/Vector/Vector2.hpp
        ../Engine/Vector/Vector3.hpp
        ../Engine/Commons/Components/Transform.hpp
        ../Engine/Rect.hpp
        ../Engine/Commons/Components/Collider.hpp
        ../Engine/Commons/Systems.hpp
        ../Engine/System/SystemTypes.hpp
        ../Engine/Hashing/IHashable.hpp
        ../Engine/Network/Packets/SeamlessTravel.hpp
        ../Engine/Hashing/Hash.hpp
        ../Engine/Network/Packets/SpawnEntity.hpp
        ../Engine/Containers/Association.hpp
        ../Engine/Reflection/Property.hpp
        ../Engine/Network/ObjectReplicator.hpp
        ../Engine/Reflection/Class.hpp
        ../Engine/Containers/Dictionary.hpp
        ../Engine/Containers/Buffer.hpp
        ../Engine/Core.hpp
        ../Engine/Reflection/Reflection.hpp
        ../Engine/Types.hpp
        ../Engine/Physics/GravitySettings.hpp
        ../Engine/Commons/Components/RigidBody.hpp
        ../Engine/Commons/Components/Scroll.hpp
        ../Engine/Network/Packets/ReplicateProperty.hpp
        ../Engine/Reflection/Rpc.hpp
        ../Engine/Network/Packets/Rpc.hpp
        ../Engine/Network/NetReliability.hpp
        ../Engine/Network/ABN/includes/Abn.hpp
        ../Engine/Network/ABN/includes/Client.hpp
        ../Engine/Network/ABN/includes/Misc.hpp
        ../Engine/Network/ABN/includes/Server.hpp
        ../Engine/FixedSizeString.hpp
        ../Engine/Math.hpp
        ../Engine/Random.hpp
        ../Engine/Player.hpp
        ../Engine/Commons/Components/VelocityComponent.hpp
        ../Engine/Network/Packets/DestroyEntity.hpp

        ../Engine/Engine.cpp
        ../Engine/Library/DynamicLibrary.cpp
        ../Engine/Library/LibLoadingAPI.cpp
        ../Engine/Containers/SparseArray.cpp
        ../Engine/Registry.cpp
        ../Engine/Entity.cpp
        ../Engine/System/Query.cpp
        ../Engine/World.cpp
        ../Engine/Logging/DefaultLogger.cpp
        ../Engine/Exceptions/Exception.cpp
        ../Engine/NetRoles.cpp
        ../Engine/Module/ModuleInfo.cpp
        ../Engine/BuildTarget.cpp
        ../Engine/System/System.cpp
        ../Engine/main.cpp
        ../Engine/GeneriqueEntity/ButtonEntity.cpp
        ../Engine/GeneriqueEntity/InputEntity.cpp
        ../Engine/display/sfml/SfmlDisplay.cpp
        ../Engine/display/sfml/SfmlEncap.cpp
        ../Engine/display/Lua/Lua.cpp
        ../Engine/Stacktrace.cpp
        ../Engine/Level/Level.cpp
        ../Engine/Network/NetId.cpp
        ../Engine/Network/INetwork.cpp
        ../Engine/Network/INetwork.h
        ../Engine/Network/Packets/NetPacket.cpp
        ../Engine/Commons/Systems.cpp
        ../Engine/Hashing/IHashable.cpp
        ../Engine/Network/Packets/SeamlessTravel.cpp
        ../Engine/Hashing/Hash.cpp
        ../Engine/Network/Packets/SpawnEntity.cpp
        ../Engine/Containers/Association.cpp
        ../Engine/Reflection/Property.cpp
        ../Engine/Network/ObjectReplicator.cpp
        ../Engine/Reflection/Class.cpp
        ../Engine/Containers/Dictionary.cpp
        ../Engine/Containers/Buffer.cpp
        ../Engine/Commons/Components/Transform.cpp
        ../Engine/Physics/GravitySettings.cpp
        ../Engine/Commons/Components/RigidBody.cpp
        ../Engine/Commons/Components/Scroll.cpp
        ../Engine/Network/Packets/ReplicateProperty.cpp
        ../Engine/Reflection/Rpc.cpp
        ../Engine/Network/Packets/Rpc.cpp
        ../Engine/Network/NetReliability.cpp
        ../Engine/Network/ABN/sources/Client/Client.cpp
        ../Engine/Network/ABN/sources/Client/Is.cpp
        ../Engine/Network/ABN/sources/Client/Listen.cpp
        ../Engine/Network/ABN/sources/Client/Send.cpp
        ../Engine/Network/ABN/sources/Client/SetGet.cpp
        ../Engine/Network/ABN/sources/Server/Is.cpp
        ../Engine/Network/ABN/sources/Server/Listen.cpp
        ../Engine/Network/ABN/sources/Server/Send.cpp
        ../Engine/Network/ABN/sources/Server/Server.cpp
        ../Engine/Network/ABN/sources/Server/SetGet.cpp
        ../Engine/Random.cpp
        ../Engine/Player.cpp
        ../Engine/Commons/Components/VelocityComponent.cpp
        ../Engine/Commons/Components/Collider.cpp
        ../Engine/Network/Packets/DestroyEntity.cpp
        ../Engine/Room/RoomData.hpp
        ../Engine/Room/RoomData.cpp
        ../Engine/Room/RoomPacket.hpp

        Client/RomSet.cpp
        Client/RomSet.hpp
        Component/SpaceShip.cpp
        Component/AutoDestroy.cpp
        Component/Enemy.cpp
        Component/Background.cpp
        Component/TekkenUtils.cpp
        Component/ComponentTekken.hpp
        Component/Projectile.cpp
        Component/Jump.cpp
        Component/Attack.cpp
        Client/LoadingLevel.cpp
        Client/LoadingLevel.hpp
        Client/LobbyLevel.cpp
        Client/LobbyLevel.hpp
        Client/GameLevel.cpp
        Client/GameLevel.hpp
        Client/SetServerAddr.hpp
        Client/SetServerAddr.cpp
        Client/Settings.hpp
        Client/Settings.cpp
        Client/CreditLevel.hpp
        Client/CreditLevel.cpp
        Client/EndGameLevel.hpp
        Client/EndGameLevel.cpp
        Systems/Systems.cpp
        Systems/Systems.h
        Server/ServerLobby.hpp
        Server/ServerLobby.cpp
        Server/ServerGame.hpp
        Server/ServerGame.cpp
        Server/ServerGameLuaScript.cpp
        Server/ServerGameLuaScript.cpp
        Server/ServerGameSender.cpp
        Network/NetworkReceiver.cpp
        Network/ClientNetworkSend.cpp
        Network/GameNetwork.hpp
        Network/ClientNetworkReceiver.cpp
        Network/GameNetworkEnum.hpp
        Network/ServerNetworkSend.cpp
        Network/ServerNetworkReceiver.cpp
)

add_executable(tekken-client
        ${RTYPE_SOURCES}
        ../Engine/Network/Packets/EnableEntityPacket.cpp
        ../Engine/Network/Packets/EnableEntityPacket.h
        Client/RomSet.cpp
)
target_compile_definitions(tekken-client PUBLIC CLIENT_BUILD)

add_executable(tekken-server
        ${RTYPE_SOURCES}
)
target_compile_definitions(tekken-server PUBLIC SERVER_BUILD)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(tekken-client PUBLIC -fsanitize=address)
        target_compile_options(tekken-server PUBLIC -fsanitize=address)
        target_link_libraries(tekken-client PRIVATE asan)
        target_link_libraries(tekken-server PRIVATE asan)
    endif ()
endif()
message(STATUS "Binary directories: ${CMAKE_BINARY_DIR}}")

set(LIBS_TO_LINK
        Threads::Threads
        sfml-graphics sfml-window sfml-audio sfml-network sfml-system
        lua
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(tekken-client PRIVATE stdc++exp)
    target_link_libraries(tekken-server PRIVATE stdc++exp)
endif()

set(DIRECTORIES_TO_INCLUDE
        ../Engine/
        ${LUA_INCLUDE_DIR}
        ${ASIO_INCLUDE_DIR}
        ${SFML_INCLUDE_DIRS}
)

target_include_directories(tekken-server PUBLIC
        ${DIRECTORIES_TO_INCLUDE}
)
target_include_directories(tekken-client PUBLIC
        ${DIRECTORIES_TO_INCLUDE}
)

target_link_libraries(tekken-client PUBLIC
        ${LIBS_TO_LINK}
)
target_link_libraries(tekken-server PUBLIC
        ${LIBS_TO_LINK}
)

if(WIN32)
    file(GLOB SFML_DLLS
            "${SFML_BINARY_DIR}/lib/*.dll"
            "${SFML_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/*.dll"
    )

    add_custom_command(TARGET tekken-client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SFML_BINARY_DIR}/lib $<TARGET_FILE_DIR:tekken-client>)

    if (EXISTS "${SFML_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
        add_custom_command(TARGET tekken-client POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SFML_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                $<TARGET_FILE_DIR:tekken-client>)
    endif ()

    add_custom_command(TARGET tekken-client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_BINARY_DIR}/_deps/sfml-src/extlibs/bin/x64"
            $<TARGET_FILE_DIR:tekken-client>)
endif()

add_custom_command(TARGET tekken-client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:tekken-client>/Assets
        COMMENT "Copying Assets to build directory"
)

add_custom_command(TARGET tekken-client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/LevelConfiguration $<TARGET_FILE_DIR:tekken-client>/LevelConfiguration
        COMMENT "Copying Lua scripts to client build directory"
)

add_custom_command(TARGET tekken-server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/LevelConfiguration $<TARGET_FILE_DIR:tekken-server>/LevelConfiguration
        COMMENT "Copying Lua scripts to server build directory"
)