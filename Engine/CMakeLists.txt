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
        Engine.cpp
        Engine.hpp
        Library/DynamicLibrary.hpp
        Library/DynamicLibrary.cpp
        Library/LibLoadingAPI.cpp
        Containers/SparseArray.cpp
        Containers/SparseArray.hpp
        Registry.cpp
        Registry.hpp
        Entity.hpp
        Entity.cpp
        System/Query.cpp
        System/Query.hpp
        World.cpp
        World.hpp
        NetRoles.hpp
        Bitflag.hpp
        Rendering/IRenderer.hpp
        Logging/ILogger.hpp
        Logging/DefaultLogger.cpp
        Logging/DefaultLogger.hpp
        Asserts.hpp
        Exceptions/Exception.cpp
        Exceptions/Exception.hpp
        NetRoles.cpp
        Module/IModule.hpp
        Module/ModuleInfo.cpp
        Module/ModuleInfo.hpp
        BuildTarget.cpp
        System/System.cpp
        System/System.hpp
        main.cpp
        Level/ILevel.hpp
        GeneriqueEntity/ButtonEntity.cpp
        GeneriqueEntity/InputEntity.cpp
        GeneriqueEntity/GeneriqueComponent.hpp
        display/sfml/SfmlDisplay.cpp
        display/sfml/SfmlDisplay.hpp
        display/sfml/SfmlEncap.cpp
        display/IDisplay.hpp
        display/Lua/Lua.cpp
        display/Lua/Lua.hpp
        Stacktrace.hpp
        Stacktrace.cpp
        ../rtype/component/r-type-utils-for-systeme.cpp
        ../rtype/Client/LoadingLevel.cpp
        ../rtype/Client/LoadingLevel.hpp
        ../rtype/Client/LobbyLevel.cpp
        ../rtype/Client/LobbyLevel.hpp
        ../rtype/Client/GameLevel.cpp
        ../rtype/Client/GameLevel.hpp
        ../rtype/Client/SetServerAddr.hpp
        ../rtype/Client/SetServerAddr.cpp
        ../rtype/Client/Settings.hpp
        ../rtype/Client/Settings.cpp
        ../rtype/Client/CreditLevel.hpp
        ../rtype/Client/CreditLevel.cpp
        ../rtype/Client/EndGameLevel.hpp
        ../rtype/Client/EndGameLevel.cpp
        Level/Level.cpp
        Level/Level.hpp
        EngineObject.hpp
        Network/NetId.cpp
        Network/NetId.hpp
        Network/INetwork.cpp
        Network/INetwork.h
        Network/Packets/NetPacket.hpp
        Network/Packets/NetPacket.cpp
        Vector/Vector2.hpp
        Vector/Vector3.hpp
        Commons/Components/Transform.hpp
        Rect.hpp
        Commons/Components/Collider.hpp
        Commons/Systems.cpp
        Commons/Systems.hpp
        System/SystemTypes.hpp
        Hashing/IHashable.cpp
        Hashing/IHashable.hpp
        Network/Packets/SeamlessTravel.cpp
        Network/Packets/SeamlessTravel.hpp
        Hashing/Hash.cpp
        Hashing/Hash.hpp
        Network/Packets/SpawnEntity.cpp
        Network/Packets/SpawnEntity.hpp
        Containers/Association.cpp
        Containers/Association.hpp
        Reflection/Property.hpp
        Reflection/Property.cpp
        Network/ObjectReplicator.cpp
        Network/ObjectReplicator.hpp
        Reflection/Class.cpp
        Reflection/Class.hpp
        Containers/Dictionary.cpp
        Containers/Dictionary.hpp
        Containers/Buffer.cpp
        Containers/Buffer.hpp
        Core.hpp
        Reflection/Reflection.hpp
        Commons/Components/Transform.cpp
        Types.hpp
        Physics/GravitySettings.cpp
        Physics/GravitySettings.hpp
        Commons/Components/RigidBody.cpp
        Commons/Components/RigidBody.hpp
        Commons/Components/Scroll.cpp
        Commons/Components/Scroll.hpp
        Network/Packets/ReplicateProperty.cpp
        Network/Packets/ReplicateProperty.hpp
        Reflection/Rpc.cpp
        Reflection/Rpc.hpp
        Network/Packets/Rpc.cpp
        Network/Packets/Rpc.hpp
        Network/NetReliability.cpp
        Network/NetReliability.hpp
        Network/ABN/includes/Abn.hpp
        Network/ABN/includes/Client.hpp
        Network/ABN/includes/Misc.hpp
        Network/ABN/includes/Server.hpp
        Network/ABN/sources/Client/Client.cpp
        Network/ABN/sources/Client/Is.cpp
        Network/ABN/sources/Client/Listen.cpp
        Network/ABN/sources/Client/Send.cpp
        Network/ABN/sources/Client/SetGet.cpp
        Network/ABN/sources/Server/Is.cpp
        Network/ABN/sources/Server/Listen.cpp
        Network/ABN/sources/Server/Send.cpp
        Network/ABN/sources/Server/Server.cpp
        Network/ABN/sources/Server/SetGet.cpp
        ../rtype/server/ServerLobby.hpp
        ../rtype/server/ServerLobby.cpp
        ../rtype/server/ServerGame.hpp
        ../rtype/server/ServerGame.cpp
        ../rtype/server/ServerGameLuaScript.cpp
        FixedSizeString.hpp
        Math.hpp

        Random.hpp
        Random.cpp

        Player.hpp
        Player.cpp
        ../rtype/Systems/Systems.cpp
        ../rtype/Systems/Systems.h
        Commons/Components/VelocityComponent.cpp
        Commons/Components/VelocityComponent.hpp
        Commons/Components/Collider.cpp
        Network/Packets/DestroyEntity.cpp
        Network/Packets/DestroyEntity.hpp
        ../rtype/server/ServerGameLuaScript.cpp
        ../rtype/server/ServerGameSender.cpp
        ../rtype/Client/RomSet.cpp
        ../rtype/Client/RomSet.hpp
        ../rtype/Client/RomSet.cpp
        ../rtype/component/Enemy.cpp
        ../rtype/component/SpaceShip.cpp
        ../rtype/component/Projectile.cpp
        ../rtype/component/Background.cpp
        ../rtype/component/AutoDestroy.cpp
        ../rtype/Network/GameNetwork.hpp
        Network/Packets/EnableEntityPacket.cpp
        Network/Packets/EnableEntityPacket.h
        ../rtype/Network/GameNetwork.hpp
        ../rtype/Network/ServerNetworkSend.cpp
        ../rtype/Network/ClientNetworkSend.cpp
        ../rtype/Network/NetworkReceiver.cpp
        ../rtype/Network/ClientNetworkReceiver.cpp
        ../rtype/Network/ServerNetworkReceiver.cpp
        Room/RoomData.hpp
        Room/RoomData.cpp
        Room/RoomPacket.hpp
        ../rtype/component/Life.cpp
        ../rtype/component/Score.cpp
        ../rtype/component/ProjectileAnimation.cpp
        ../rtype/component/EnemyProjectile.cpp
)

add_executable(rtype-client
        ${RTYPE_SOURCES}

)

target_compile_definitions(rtype-client PUBLIC CLIENT_BUILD)

add_executable(rtype-server
        ${RTYPE_SOURCES}
)
target_compile_definitions(rtype-server PUBLIC SERVER_BUILD)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(rtype-client PUBLIC -fsanitize=address)
        target_compile_options(rtype-server PUBLIC -fsanitize=address)
        target_link_libraries(rtype-client PRIVATE asan)
        target_link_libraries(rtype-server PRIVATE asan)
    endif ()
endif()
message(STATUS "Binary directories: ${CMAKE_BINARY_DIR}}")

set(LIBS_TO_LINK
        Threads::Threads
        sfml-graphics sfml-window sfml-audio sfml-network sfml-system
        lua
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(rtype-client PRIVATE stdc++exp)
    target_link_libraries(rtype-server PRIVATE stdc++exp)
endif()

set(DIRECTORIES_TO_INCLUDE
        .
        ${LUA_INCLUDE_DIR}
        ${ASIO_INCLUDE_DIR}
        ${SFML_INCLUDE_DIRS}
)

target_include_directories(rtype-server PUBLIC
        ${DIRECTORIES_TO_INCLUDE}
)
target_include_directories(rtype-client PUBLIC
        ${DIRECTORIES_TO_INCLUDE}
)

target_link_libraries(rtype-client PUBLIC
    ${LIBS_TO_LINK}
)
target_link_libraries(rtype-server PUBLIC
    ${LIBS_TO_LINK}
)

if(WIN32)
    file(GLOB SFML_DLLS
            "${SFML_BINARY_DIR}/lib/*.dll"
            "${SFML_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/*.dll"
    )

    add_custom_command(TARGET rtype-client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SFML_BINARY_DIR}/lib $<TARGET_FILE_DIR:rtype-client>)

    if (EXISTS "${SFML_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")
        add_custom_command(TARGET rtype-client POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SFML_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                $<TARGET_FILE_DIR:rtype-client>)
    endif ()

    add_custom_command(TARGET rtype-client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_BINARY_DIR}/_deps/sfml-src/extlibs/bin/x64"
            $<TARGET_FILE_DIR:rtype-client>)
endif()

add_custom_command(TARGET rtype-client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:rtype-client>/Assets
        COMMENT "Copying Lua scripts to build directory")

add_custom_command(TARGET rtype-client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/../rtype/LevelConfiguration $<TARGET_FILE_DIR:rtype-client>/LevelConfiguration
        COMMENT "Copying Lua scripts to build directory")

add_custom_command(TARGET rtype-server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/../rtype/LevelConfiguration $<TARGET_FILE_DIR:rtype-server>/LevelConfiguration
        COMMENT "Copying Lua scripts to build directory")
