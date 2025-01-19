//
// Created by Nail on 18/11/2024.
//

#ifndef QUERY_HPP
#define QUERY_HPP

#include <typeindex>
#include <vector>

namespace MGE
{
    /**
     * Represents a registry query.
     */
    class Query
    {
        public:
            /**
             * Initializes a new, empty query.
             */
            Query() = default;
            Query(const Query &other) noexcept = default;
            Query(Query &&other) noexcept;

            Query& operator=(const Query &other) noexcept = default;

            /**
             * Checks query equality. Two queries are deemed equal if they include the same components.
             * @note Two queries that are equal are guaranteed to give the same result. Two queries that are not equal may still give the same result.
             * @return true if those queries include the same components, false otherwise.
             */
            bool operator==(const Query &other) const;

            /**
             * Add types to this query.
             * @returns This query.
             * @todo doesn't prevent from adding twice the same component type.
             */
            template<typename... TComponents>
            Query &IncludeTypes();

            /**
             * Checks if this query is empty.
             * @return true if this query is empty, false otherwise.
             */
            [[nodiscard]] bool IsEmpty() const;

            int operator<(const Query &other) const;

        private:
            std::vector<std::type_index> includedComponentTypes;
    };

    template<typename ... TComponents>
    Query & Query::IncludeTypes()
    {
        (includedComponentTypes.emplace_back(typeid(TComponents)), ...);
        return *this;
    }
} // MGE

#endif //QUERY_HPP
