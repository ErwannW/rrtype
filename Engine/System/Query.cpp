//
// Created by Nail on 18/11/2024.
//

#include "Query.hpp"

#include <algorithm>
#include <typeindex>

namespace MGE
{
    Query::Query(Query &&other) noexcept : includedComponentTypes(std::move(other.includedComponentTypes))
    {

    }

    bool Query::operator==(const Query &other) const
    {
        return other.includedComponentTypes.size() != includedComponentTypes.size()
            && std::ranges::all_of(includedComponentTypes, [&other](const std::type_index &comp) -> bool {
                return std::ranges::find(other.includedComponentTypes, comp) == other.includedComponentTypes.end();
            });
    }

    bool Query::IsEmpty() const
    {
        return includedComponentTypes.empty();
    }

    int Query::operator<(const Query &other) const
    {
        return includedComponentTypes.size() < other.includedComponentTypes.size();
    }
} // MGE