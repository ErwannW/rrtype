//
// Created by np on 11/29/24.
//

#ifndef RTYPE_OBJECTREPLICATOR_HPP
#define RTYPE_OBJECTREPLICATOR_HPP

#include <map>
#include "Engine.hpp"
#include "Hashing/Hash.hpp"
#include "Reflection/Property.hpp"
#include "Asserts.hpp"
#include "NetId.hpp"
#include "Reflection/Class.hpp"
#include "Containers/Dictionary.hpp"
#include "Containers/Buffer.hpp"

namespace MGE {

    struct PropertyTracker
    {
        explicit PropertyTracker(const Property& property);

        const Property& property;

        Buffer& GetStateSaveBuffer();
        const Buffer& GetStateSaveBuffer() const;

        private:
            Buffer lastState;
    };

    /**
     * @struct ObjectChangesTracker
     * Struct for tracking the property changes of an object using the MGE's reflection system.
     */
    struct ObjectChangesTracker
    {
        public:
            ObjectChangesTracker(ObjectChangesTracker&& other) noexcept;

            template<typename T>
            explicit ObjectChangesTracker(T* obj, NetId netId);

            ObjectChangesTracker(const Class& objClass, void* obj, NetId netId);

            ObjectChangesTracker& operator=(ObjectChangesTracker&& other) noexcept;

            /**
             * Clear all changes that was saved.
             */
            void ResetSavedState();

            /**
             * Reverts all changed that was made to the replicated properties of the object since the last state reset.
             */
            void RevertAllChanges() const;

            /**
             * @return The object that is being tracked.
             */
            [[nodiscard]] constexpr void* GetTarget() const noexcept;

            /**
             * @return The list of properties of this object that has changed.
             */
            [[nodiscard]] std::vector<std::reference_wrapper<const Property>> GetChangedProperties() const;

            [[nodiscard]] NetId GetNetId() const;

        private:
            /**
             * The class of the object that is being tracked.
             */
            const Class* _class;
            bool _hasSavedState = false;
            std::byte* target;
            std::vector<PropertyTracker> propertyTrackers;
            NetId netId;
    };

    template<typename T>
    ObjectChangesTracker::ObjectChangesTracker(T *obj, NetId netId) : ObjectChangesTracker(T::GetClass(), obj, netId)
    {

    }

    constexpr void *ObjectChangesTracker::GetTarget() const noexcept
    {
        return target;
    }

    constexpr bool operator==(const ObjectChangesTracker& a, const ObjectChangesTracker& b)
    {
        return a.GetTarget() == b.GetTarget();
    }

    struct ObjectReplicator
    {
        void TrackObject(const Class& objectClass, void* obj, NetId netId)
        {
            if (!VERIFY(objectClass.ShouldReplicate(), "Cannot track object of type \"" + objectClass.GetName() + "\" that has no replicated properties"))
                return;

            if (obj == nullptr)
                return;

            if (std::ranges::find_if(trackedObjects, [obj](const ObjectChangesTracker& objsi) -> bool {
                return objsi.GetTarget() == obj;
            }) != trackedObjects.end())
                return;

            trackedObjects.emplace_back(objectClass, obj, netId);
        }

        void StopTracking(const void* obj)
        {
            if (obj == nullptr)
                return;

            std::erase_if(trackedObjects, [obj](const ObjectChangesTracker& o) -> bool {
                return o.GetTarget() == obj;
            });
        }

        void StopTracking(const NetId netId)
        {
            std::erase_if(trackedObjects, [netId](const ObjectChangesTracker& o) -> bool {
                return o.GetNetId() == netId;
            });
        }

        [[nodiscard]] std::vector<ObjectChangesTracker> &GetTrackedObjects() noexcept;
        [[nodiscard]] const std::vector<ObjectChangesTracker> &GetTrackedObjects() const noexcept;

        private:
            std::vector<ObjectChangesTracker> trackedObjects;
    };

} // MGE

#endif // RTYPE_OBJECTREPLICATOR_HPP
