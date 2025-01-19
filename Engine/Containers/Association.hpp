//
// Created by np on 11/29/24.
//

#ifndef RTYPE_ASSOCIATION_HPP
#define RTYPE_ASSOCIATION_HPP

#include <utility>
#include <vector>
#include <stdexcept>

namespace MGE
{
    /**
     * @struct Association
     * Associates a unique key to a unique value.
     * @tparam TFirst
     * @tparam TSecond
     */
    template<typename TFirst, typename TSecond>
    class Association
    {
        static_assert(!std::is_same_v<TFirst, TSecond>, "TFirst and TSecond cannot be the same type.");

        public:
            using Type = std::pair<TFirst, TSecond>;
            using ContainerType = std::vector<Type>;

            using iterator = typename ContainerType::iterator;

            /**
             * Get the value associated with this one.
             * @param a
             * @return
             */
            TFirst &operator[](const TSecond& a);
            /**
             * Get the value associated with this one.
             * @param a
             * @return
             */
            TSecond &operator[](const TFirst& a);

            /**
             * Get the value associated with this one.
             * @param a
             * @return
             */
            TFirst &at(const TSecond& a);
            /**
             * Get the value associated with this one.
             * @param b
             * @return
             */
            TSecond &at(const TFirst& b);

            /**
             * Get the value associated with this one.
             * @param a
             * @return
             */
            const TFirst &at(const TSecond& a) const;
            /**
             * Get the value associated with this one.
             * @param b
             * @return
             */
            const TSecond &at(const TFirst& b) const;

            /**
             * Find the associated value of the given one.
             * @param val
             * @return An iterator to the position of the pair.
             */
            iterator find(const TFirst& val);

            /**
             * Find the associated value of the given one.
             * @param val 
             * @return An iterator to the position of the pair.
             */
            iterator find(const TSecond& val);

        private:
            std::vector<std::pair<TFirst, TSecond>> container;
    };

    template<typename TFirst, typename TSecond>
    TFirst &Association<TFirst, TSecond>::at(const TSecond &a)
    {
        auto it = find(a);

        if (it == container.end())
            throw std::out_of_range("wtf bro");
        return it->first;
    }

    template<typename TFirst, typename TSecond>
    const TFirst &Association<TFirst, TSecond>::at(const TSecond &a) const
    {
        auto it = find(a);

        if (it == container.end())
            throw std::out_of_range("wtf bro");
        return it->first;
    }

    template<typename TFirst, typename TSecond>
    TSecond &Association<TFirst, TSecond>::at(const TFirst &b)
    {
        auto it = find(b);

        if (it == container.end())
            throw std::out_of_range("good try lil bro");
        return it->second;
    }

    template<typename TFirst, typename TSecond>
    const TSecond &Association<TFirst, TSecond>::at(const TFirst &b) const
    {
        auto it = find(b);

        if (it == container.end())
            throw std::out_of_range("good try lil bro");
        return it->second;
    }

    template<typename TFirst, typename TSecond>
    typename Association<TFirst, TSecond>::iterator Association<TFirst, TSecond>::find(const TFirst &val)
    {
        for (auto it = container.begin(); it != container.end(); ++it)
        {
            if (it->first == val)
                return it;
        }
        return container.end();
    }

    template<typename TFirst, typename TSecond>
    typename Association<TFirst, TSecond>::iterator Association<TFirst, TSecond>::find(const TSecond &val)
    {
        for (auto it = container.begin(); it != container.end(); ++it)
        {
            if (it->second == val)
               return it;
        }
        return container.end();
    }

    template<typename TFirst, typename TSecond>
    TFirst &Association<TFirst, TSecond>::operator[](const TSecond &a)
    {
        iterator it = find(a);

        if (it == container.end())
            return container.emplace_back().first;
        return it->first;
    }

    template<typename TFirst, typename TSecond>
    TSecond &Association<TFirst, TSecond>::operator[](const TFirst &a)
    {
        iterator it = find(a);

        if (it == container.end())
            return container.emplace_back().second;
        return it->second;
    }

} // MGE

#endif //RTYPE_ASSOCIATION_HPP
