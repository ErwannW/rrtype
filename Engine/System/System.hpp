//
// Created by Nail on 21/11/2024.
//

#ifndef SYSTEM2_HPP
#define SYSTEM2_HPP

#include <functional>
#include "System/SystemTypes.hpp"
#include "Query.hpp"
#include "Registry.hpp"
#include "System.hpp"
#include "World.hpp"

namespace MGE
{
    class Engine;

    /**
     * @struct RunPolicy
     * A struct to manage the running policy of the system.
     */
    struct RunPolicy {
        constexpr RunPolicy(const bool client, const bool server) noexcept : client(client), server(server)
        {

        }
        constexpr RunPolicy(const RunPolicy& other) noexcept = default;
        constexpr RunPolicy(RunPolicy&& other) noexcept = default;
        constexpr RunPolicy &operator=(const RunPolicy& other) noexcept = default;

        /**
         * Should this system run on the clients?
         */
        bool client = true;
        /**
         * Should this system run on the server?
         */
        bool server = true;
    };

    /**
     * @class System
     * Represents a function that run on all entities that contains the components it is registered to run on.
     */
    class System : public IEngineObject
    {
        public:
            inline static RunPolicy ClientOnly      = RunPolicy(true, false);
            inline static RunPolicy ServerOnly      = RunPolicy(false, true);
            inline static RunPolicy ClientAndServer = RunPolicy(true, true);

            template<typename... TComponents>
            System(std::string name, SystemFuncPtr<TComponents...> func, RunPolicy runPolicy = ClientAndServer, int order = 0);

            System(const System& other) = default;
            System& operator=(const System& other) = default;

            void operator()(Registry&r) const;

            /**
             * @return The name of this system.
             */
            [[nodiscard]] std::string_view GetName() const override;

            /**
             * The execution order of this system.
             * The higher the execution order is, the latter it will be executed.
             * @note The order is relative to other systems. Two components can have the same order.
             */
            std::int32_t order = 0;

            /**
             * Is this system enabled ?
             */
            bool enabled = true;

            /**
             * The run policy of this system.
             */
            RunPolicy runPolicy = ClientAndServer;

            /**
             * Checks if this system should run on this engine instance.
             * @param engine
             * @return
             */
            [[nodiscard]] bool CanRunOn(const Engine& engine) const;

            template<typename... TComponents>
            [[nodiscard]] bool operator==(SystemFuncPtr<TComponents...> systemPtr) const noexcept;

            /**
             *
             * @return A pointer to the function pointer that is being ran by this system.
             */
            [[nodiscard]] const void* TargetSystemFunctionPtr() const noexcept;

            /**
             * @return The query that is executed by this system to find its entities.
             */
            [[nodiscard]] const Query& GetQuery() const;
        private:
            std::string name;
            Query query;
            std::function<void(Registry&)> func;

            void* funcAddr;
    };

    template<typename... TComponents>
    System::System(std::string name, SystemFuncPtr<TComponents...> func, RunPolicy runPolicy, const int order) : order(order), runPolicy(runPolicy), name(std::move(name)), funcAddr(reinterpret_cast<void*>(func))
    {
        this->func = [func, this](Registry &r) {
            static_assert(sizeof...(TComponents) > 0, "A system must run on at least one component.");

            const std::size_t max = GetSize<TComponents...>(r);
            for (std::size_t i = 0; i < max; ++i) {
                const auto &entity = r.GetEntityByComponentId(i);
                if (!Entity::IsValidAndEnabled(entity))
                    continue;

                if ((... && r.GetComponents<TComponents>()[i].has_value()))
                    func(r.world, r.GetComponents<TComponents>()[i].value()...);
            }
        };
    }

    template<typename ... TComponents>
    bool System::operator==(SystemFuncPtr<TComponents...> systemPtr) const noexcept
    {
        return func == systemPtr;
    }
} // MGE

#endif //SYSTEM2_HPP
