//
// Created by Nail on 18/11/2024.
//

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>
#include <filesystem>
#include "Types.hpp"
#include "Network/ABN/includes/Abn.hpp"
#include "Library/DynamicLibrary.hpp"
#include "NetRoles.hpp"
#include "display/IDisplay.hpp"
#include "Rendering/IRenderer.hpp"
#include "Logging/ILogger.hpp"
#include "Room/RoomData.hpp"

#include <BuildTarget.hpp>

namespace MGE
{
    class World;

    /**
     * This is the main engine class. Each running instance of the game can only have one instance of this class.
     */
    class Engine
    {
        public:
            explicit Engine();
            explicit Engine(std::vector<std::string> &&args);
            ~Engine();

            std::shared_ptr<World> world;

            /**
            * Start the engine and the game.
            */
            uint8 Start();

            /**
             * Loads the given library.
             * @param libPath The path to the dynamic library to load.
             * @return true if the loading was successful, false otherwise.
             */
            bool UseLib(const std::filesystem::path &libPath);

            template<typename TRenderer = IRenderer>
            [[nodiscard]] bool HasRenderer() const noexcept;

            template<typename TRenderer = IRenderer, typename = std::enable_if<std::is_base_of_v<IRenderer, TRenderer>>>
            [[nodiscard]] TRenderer* GetRenderer() const;
            template<typename TRenderer, typename = std::enable_if<std::is_base_of_v<IRenderer, TRenderer>>, typename... TArgs>
            TRenderer* SetRenderer(TArgs&&... args);

            template<typename TLogger = ILogger, typename = std::enable_if<std::is_base_of_v<ILogger, TLogger>>>
            [[nodiscard]] TLogger* GetLogger() const;
            template<typename TLogger, typename = std::enable_if<std::is_base_of_v<ILogger, TLogger>>, typename... TArgs>
            TLogger* SetLogger(TArgs&&... args);

            /**
             * Requests to quit the engine.
             * @param exitCode The code to return when exiting.
             */
            void Quit(std::uint8_t exitCode = 0);

            [[nodiscard]] bool ShouldQuit() const noexcept;

            [[nodiscard]] bool IsRunning() const noexcept;

            static Engine& Current() noexcept;

            [[nodiscard]] RoomData& GetRoomData() noexcept;
        private:
            std::unique_ptr<IRenderer> renderer;
            std::unique_ptr<ILogger> logger;

            /**
             * The list of command lines arguments the user entered when launching the engine/game.
             */
            std::vector<std::string> commandLineArgs;
            /**
             * All the dynamic libs currently loaded by the engine.
             */
            std::vector<DynamicLib> libs;

            bool isRunning = false;

            /**
             * Should the engine quit at the end of this frame?
             */
            bool shouldQuit = false;
            std::uint8_t exitCode = 0;

            static Engine* current;
            RoomData roomData;
    };

    template<typename TRenderer>
    bool Engine::HasRenderer() const noexcept
    {
        return renderer != nullptr
            && dynamic_cast<TRenderer*>(renderer.get()) != nullptr;
    }

    template<typename TRenderer, typename>
    TRenderer* Engine::GetRenderer() const
    {
        if constexpr (std::is_same_v<IRenderer, TRenderer>)
            return renderer.get();

        return dynamic_cast<TRenderer*>(renderer.get());
    }

    template<typename TRenderer, typename, typename ... TArgs>
    TRenderer* Engine::SetRenderer(TArgs &&...args)
    {
        if constexpr (BuildTarget::IsServerBuild()) {
            GetLogger()->Warning("SetValue renderer is ignored on dedicated server builds.");
            return nullptr;
        }

        renderer = std::make_unique<TRenderer>(std::forward<TArgs>(args)...);
        GetRenderer<IDisplay>()->InitDisplay();
        return GetRenderer<TRenderer>();
    }

    template<typename TLogger, typename>
    TLogger* Engine::GetLogger() const
    {
        if constexpr (std::is_same_v<ILogger, TLogger>)
            return logger.get();

        return dynamic_cast<TLogger*>(logger.get());
    }

    template<typename TLogger, typename, typename ... TArgs>
    TLogger* Engine::SetLogger(TArgs &&...args)
    {
        logger = std::make_unique<TLogger>(std::forward<TArgs>(args)...);

        if (logger != nullptr)
            logger->Info("Logging started.");

        return GetLogger<TLogger>();
    }
} // MGE

#endif //ENGINE_HPP
