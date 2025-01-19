//
// Created by np on 12/2/24.
//

#ifndef RTYPE_DICTIONARY_HPP
#define RTYPE_DICTIONARY_HPP

#include <vector>
#include <utility>
#include <ranges>

namespace MGE
{
    /**
     * @struct Dictionary
     * An associative container of unique keys. TKey must have a valid operator== overload.
     * @tparam TKey
     * @tparam TValue
     */
    template<typename TKey, typename TValue>
    class Dictionary
    {
        public:
            auto begin() { return pairs.begin(); }
            auto begin() const { return pairs.begin(); }

            auto end() { return pairs.end(); }
            auto end() const { return pairs.end(); }

            /**
             * Get the value associated to this key.
             * @param key
             * @return
             */
            TValue &operator[](const TKey& key);
            /**
             * Get the value associated to this key.
             * @param key
             * @return
             */
            TValue const &operator[](const TKey& key) const;

            /**
            * Get the value associated to this key.
            * @param key
            * @return
            */
            TValue &at(const TKey &key);
            /**
             * Get the value associated to this key.
             * @param key
             * @return
             */
            const TValue &at(const TKey &key) const;

            /**
             * Creates a value by avoiding unncesary copy.
             * @param key
             */
            void Emplace(TKey&& key, TValue&& value);

            /**
             * Checks if the given key exists in the dictionary.
             * @param keyToFind
             * @return
             */
            [[nodiscard]] bool Contains(const TKey &keyToFind) const;

            /**
             * Removes all keys and values of the dictionary, leaving it in an empty state.
             */
            void Clear();

        private:
            std::vector<std::pair<TKey, TValue>> pairs;
    };

    template<typename TKey, typename TValue>
    TValue &Dictionary<TKey, TValue>::at(const TKey &key)
    {
        auto it = std::ranges::find_if(pairs, [&key](const std::pair<TKey, TValue> & pair) -> bool {
            return key == pair.first;
        });

        return it->second;
    }

    template<typename TKey, typename TValue>
    const TValue &Dictionary<TKey, TValue>::at(const TKey &key) const
    {
        auto it = std::ranges::find_if(pairs, [&key](const std::pair<TKey, TValue> & pair) -> bool {
            return key == pair.first;
        });

        return it->second;
    }

    template<typename TKey, typename TValue>
    void Dictionary<TKey, TValue>::Clear()
    {
        pairs.clear();
    }

    template<typename TKey, typename TValue>
    bool Dictionary<TKey, TValue>::Contains(const TKey &keyToFind) const
    {
      for (const std::pair<TKey, TValue> &pair : pairs)
          if (pair.first == keyToFind)
               return true;

        return false;
    }

    template<typename TKey, typename TValue>
    void Dictionary<TKey, TValue>::Emplace(TKey &&key, TValue &&value)
    {
        pairs.emplace_back(std::forward<TKey>(key), std::forward<TValue>(value));
    }

    template<typename TKey, typename TValue>
    TValue const &Dictionary<TKey, TValue>::operator[](const TKey &key) const
    {
        auto it = std::ranges::find_if(pairs, [&key](const auto& pair) -> bool {
            return pair.first == key;
        });

        return it->second;
    }

    template<typename TKey, typename TValue>
    TValue &Dictionary<TKey, TValue>::operator[](const TKey &key)
    {
        auto it = std::ranges::find_if(pairs, [&key](const std::pair<TKey, TValue>& pair) -> bool {
            return pair.first == key;
        });

        if (it == pairs.end())
            pairs.emplace_back(key, TValue());
        return pairs.back().second;
    }

} // MGE

#endif //RTYPE_DICTIONARY_HPP
