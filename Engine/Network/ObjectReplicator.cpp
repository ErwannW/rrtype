//
// Created by np on 11/29/24.
//

#include "ObjectReplicator.hpp"

namespace MGE
{
    ObjectChangesTracker::ObjectChangesTracker(ObjectChangesTracker &&other) noexcept
        : _class(other._class), _hasSavedState(other._hasSavedState), target(other.target), propertyTrackers(std::move(other.propertyTrackers)), netId(other.netId)
    {
        other.target = nullptr;
        other.netId = NetId::None;
        other._hasSavedState = false;
        other._class = nullptr;
    }

    ObjectChangesTracker::ObjectChangesTracker(const Class &objClass, void *obj, const NetId netId)
        : _class(std::addressof(objClass)), target(static_cast<std::byte *>(obj)), netId(netId)
    {
        for (const Property &property : _class->GetProperties()) {
              if (!property.ShouldReplicate())
                  continue;

              propertyTrackers.emplace_back(property);
        }
        ResetSavedState();
    }

    ObjectChangesTracker & ObjectChangesTracker::operator=(ObjectChangesTracker &&other) noexcept
    {
        if (this == std::addressof(other))
            return *this;

        _class = other._class;
        _hasSavedState = other._hasSavedState;
        target = other.target;
        propertyTrackers = std::move(other.propertyTrackers);
        netId = other.netId;

        other._class = nullptr;
        other._hasSavedState = false;
        other.target = nullptr;
        other.netId = NetId::None;
        return *this;
    }

    void ObjectChangesTracker::ResetSavedState()
    {
        for (PropertyTracker &propertyTracker : propertyTrackers)
            std::memcpy(propertyTracker.GetStateSaveBuffer(), propertyTracker.property.GetAddress(target), propertyTracker.property.GetSize());

        _hasSavedState = true;
    }

    std::vector<std::reference_wrapper<const Property>> ObjectChangesTracker::GetChangedProperties() const
    {
        std::vector<std::reference_wrapper<const Property>> changedProperties;

        for (const PropertyTracker &propertyTracker : propertyTrackers) {
            if (!_hasSavedState || std::memcmp(propertyTracker.property.GetAddress(target), propertyTracker.GetStateSaveBuffer(), propertyTracker.property.GetSize()) != 0)
                changedProperties.emplace_back(propertyTracker.property);
        }
        return changedProperties;
    }

    NetId ObjectChangesTracker::GetNetId() const {
        return netId;
    }

    void ObjectChangesTracker::RevertAllChanges() const
    {
        for (const PropertyTracker& tracker: propertyTrackers)
            tracker.property.SetValue(target, tracker.GetStateSaveBuffer());
    }

    std::vector<ObjectChangesTracker> &ObjectReplicator::GetTrackedObjects() noexcept
    {
        return trackedObjects;
    }

    const std::vector<ObjectChangesTracker> &ObjectReplicator::GetTrackedObjects() const noexcept
    {
        return trackedObjects;
    }

    PropertyTracker::PropertyTracker(const Property &property) : property(property), lastState(property.GetSize())
    {

    }

    Buffer& PropertyTracker::GetStateSaveBuffer()
    {
        return lastState;
    }

    const Buffer& PropertyTracker::GetStateSaveBuffer() const
    {
        return lastState;
    }
} // namespace MGE
