//
// Created by Nail on 18/11/2024.
//

#include <cstdint>
#include "Vector/Vector3.hpp"
#include "Core.hpp"
#include "Asserts.hpp"
#include "BuildTarget.hpp"
#include "Bitflag.hpp"
#include "Library/DynamicLibrary.hpp"
#include "Logging/DefaultLogger.hpp"
#include "Module/IModule.hpp"
#include "Exceptions/Exception.hpp"

namespace MGE
{
    Engine* Engine::current = nullptr;

    Engine::Engine()
    {
        current = this;

        world = std::make_shared<World>(*this);

        SetLogger<DefaultLogger>();
        GetLogger()->Info(std::string(BuildTarget::PlatformName()) + ' ' + ToString(BuildTarget::CurrentBuildType) + ' ' + ToString(BuildTarget::CurrentBuildConfiguration) + std::string(" build"));
    }

    Engine::Engine(std::vector<std::string> &&args) : commandLineArgs(std::move(args))
    {
        current = this;
    }

    Engine::~Engine()
    {
        for (DynamicLib &lib : libs) {
            GetLogger()->Info("Unloading lib " + lib.GetPath().string());
            lib.UnLoad();
        }
    }

    bool Engine::UseLib(const std::filesystem::path &libPath)
    {
        if (!std::filesystem::exists(libPath))
            return false;

        try {
            DynamicLib lib = DynamicLib(libPath);
            lib.Load();
            libs.emplace_back(std::move(lib));
            const std::shared_ptr<IModule> module = libs.back().Call<std::shared_ptr<IModule>()>("CreateModule");
            GetLogger()->Info("Hooked module " + module->GetModuleInfo().GetName() + " (" + ToString(module->GetModuleInfo().GetBuildConfiguration()) + ")");
        } catch (...) {
            return false;
        }
        return true;
    }

    uint8 Engine::Start()
    {
        isRunning = true;
        GetLogger()->Info("Running as " + ToString(BuildTarget::CurrentBuildType));

        while (!ShouldQuit())
        {
            if (HasRenderer())
                GetRenderer()->PreDraw();
            world->Update();
            if (HasRenderer())
                GetRenderer()->PostDraw();
        }

        GetLogger()->Info("Exiting with code " + std::to_string(exitCode) + ".");
        return exitCode;
    }

    void Engine::Quit(const std::uint8_t exitCode)
    {
        GetLogger()->Info("Quit requested.");

        shouldQuit = true;
        this->exitCode = exitCode;
    }

    bool Engine::ShouldQuit() const noexcept
    {
        return shouldQuit;
    }

    bool Engine::IsRunning() const noexcept
    {
        return isRunning;
    }

    Engine& Engine::Current() noexcept
    {
        return *current;
    }

    RoomData& Engine::GetRoomData() noexcept
    {
      return roomData;
    }
} // MGE