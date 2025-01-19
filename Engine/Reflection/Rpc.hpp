//
// Created by np on 12/6/24.
//

#ifndef RTYPE_RPC_HPP
#define RTYPE_RPC_HPP

#include <functional>
#include <typeindex>
#include "Types.hpp"
#include "Hashing/IHashable.hpp"
#include "Network/NetReliability.hpp"

#define RPC_PARAM_STRUCT(name) name##_Params
#define DECLARE_RPC(name, ...) struct RPC_PARAM_STRUCT(name) { __VA_ARGS__ __VA_OPT__(;) }; static_assert(std::is_trivially_copyable_v<RPC_PARAM_STRUCT(name)>, "All params of " #name " must be trivially copyable."); void name(const RPC_PARAM_STRUCT(name)& args);

namespace MGE
{
    template<typename TTarget, typename TArg>
    using RPCMethod = MethodPtr<TTarget, void, TArg>;

    class Class;

    /**
     * @struct RPC
     * Represents an RPC of a class for the reflection system.
     */
    class RPC
    {
        public:
            enum class Target {
                /**
                 * Default value, not a invalid target.
                 */
                None,
                /**
                 * The RPC will be called on a client, and will be executed on the server.
                 */
                Server,
                /**
                 * The RPC will be called on the server, and will be executed on the clients.
                 */
                Clients
            };

            template<typename TTarget, typename TArg>
            explicit RPC(const Class& _class, std::string name, Target target, RPCMethod<TTarget, TArg> method, NetReliability reliability);

            /**
             * The class of this RPC.
             */
            const Class& _class;
            /**
             * The target of this RPC.
             */
            const Target target;
            /**
             * The size of the param struct of the RPC.
             */
            const std::size_t paramSize;
            /**
             * The std::type_index of this RPC param.
             */
            const std::type_index paramTypeIndex;

            /**
             * The reliability of this RPC.
             */
            const NetReliability reliability;

            void operator()(void* obj, void* data) const;
            void Call(void* obj, const void *data) const;

            /**
             * @return The hash of this RPC. Note that the hash of an RPC is computed using its fullname.
             */
            [[nodiscard]] MGE::Hash GetHash() const;

            /**
             * @return The name of the RPC.
             */
            [[nodiscard]] const std::string& GetName() const;

            /**
             * @return The full name of the RPC ("ClassName::RPCName").
             */
            [[nodiscard]] std::string GetFullName() const;
        private:
            std::string name;
            MGE::Hash hash;

            std::function<void(void*, const void*)> call;
    };

    template<typename TTarget, typename TArg>
    RPC::RPC(const class Class& _class, std::string name, Target target, RPCMethod<TTarget, TArg> method, NetReliability reliability)
            : _class(_class), name(std::move(name)), target(target), paramSize(sizeof(TArg)), hash(MGE::HashString(GetFullName())),
              paramTypeIndex(typeid(TArg)),
              call([method](void* target, const void* data) -> void {
                  (reinterpret_cast<TTarget*>(target)->*method)(*reinterpret_cast<std::remove_reference_t<const TArg>*>(data));
              }), reliability(reliability)
    {

    }

    constexpr bool operator==(const RPC& rpc, const std::string& name) noexcept
    {
        return name == rpc.GetName() || name == rpc.GetFullName();
    }
} // MGE

#endif //RTYPE_RPC_HPP
