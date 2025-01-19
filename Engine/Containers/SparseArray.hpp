//
// Created by Nail on 18/11/2024.
//

#ifndef SPARSEARRAY_HPP
#define SPARSEARRAY_HPP

#include <cstring>
#include <optional>
#include <vector>
#include "Asserts.hpp"

/**
 * A struct representing a value that may or may not exists.
 * @tparam T The type of the value.
 */
template <typename T>
struct Optional
{
    Optional() = default;
    Optional(T &&value);
    Optional(std::nullopt_t);
    Optional(const Optional<T> &other);
    Optional(Optional<T> &&other) noexcept;

    T &value();
    T &operator*();
    Optional<T> &operator=(T &&value);
    Optional<T> &operator=(const T &value);
    Optional<T> &operator=(std::nullopt_t);
    Optional<T> &operator=(const Optional<T>& other);
    Optional<T> &operator=(Optional<T>&& other) noexcept;

    /**
     * Clears the value.
     */
    void reset();

    /**
     * Check if a value exists.
     * @return true if the optional has a value, false otherwise.
     */
    [[nodiscard]] bool has_value() const;

    /**
     * Emplaces the value with the given args, avoiding unnecessary copies.
     * @tparam TArgs
     * @param args
     */
    template<typename... TArgs>
    void emplace(TArgs&&... args);

    private:
        std::unique_ptr<T> _value = nullptr;
};

template<typename T>
Optional<T> & Optional<T>::operator=(const Optional<T> &other)
{
    if (std::addressof(other) == this)
        return *this;

    if (other.has_value())
        _value = std::make_unique<T>(*other._value);
    else
        reset();
    return *this;
}

template<typename T>
Optional<T> & Optional<T>::operator=(Optional<T> &&other) noexcept
{
    if (std::addressof(other) == this)
        return *this;

    _value = std::move(other._value);
    return *this;
}

template<typename T>
Optional<T>::Optional(T &&value)
{
    _value = std::make_unique<T>(std::forward<T>(value));
}

template<typename T>
Optional<T>::Optional(std::nullopt_t) : _value(nullptr)
{

}

template<typename T>
Optional<T>::Optional(const Optional<T> &other)
{
    if (other.has_value())
        _value = std::make_unique<T>(*other._value);
}

template<typename T>
Optional<T>::Optional(Optional<T> &&other) noexcept
{
    _value = std::move(other._value);
}

template<typename T>
T & Optional<T>::value()
{
    // EXPECT(_value != nullptr, "Trying to access non-existent value");
    if (_value == nullptr)
        std::terminate();
    return *_value;
}

template<typename T>
T & Optional<T>::operator*()
{
    return value();
}

template<typename T>
Optional<T> & Optional<T>::operator=(T &&value)
{
    _value = std::make_unique<T>(std::forward<T>(value));
    return *this;
}

template<typename T>
Optional<T> & Optional<T>::operator=(const T &value)
{
    _value = std::make_unique<T>(value);
    return *this;
}

template<typename T>
Optional<T> & Optional<T>::operator=(std::nullopt_t)
{
    reset();
    return *this;
}

template<typename T>
void Optional<T>::reset()
{
    _value = nullptr;
}

template<typename T>
bool Optional<T>::has_value() const
{
    return _value != nullptr;
}

template<typename T>
template<typename ... TArgs>
void Optional<T>::emplace(TArgs &&...args)
{
    _value = std::make_unique<T>(std::forward<TArgs>(args)...);
}

namespace MGE
{
    template <typename TComponent>
    class SparseArray
    {
        public:
            template<typename T>
            using optional_type = Optional<T>;

            using container_t = std::vector<optional_type<TComponent>>;

            using value_type = typename container_t::value_type;
            using reference_type = typename container_t::reference;
            using const_reference_type = typename container_t::const_reference;
            using size_type = typename container_t::size_type;
            using iterator = typename container_t::iterator;
            using const_iterator = typename container_t::const_iterator;

            SparseArray() = default;
            SparseArray(SparseArray const &other) = default;
            SparseArray(SparseArray &&other) noexcept;
            ~SparseArray() = default;

            SparseArray &operator =(SparseArray const &other) = default;
            SparseArray &operator =(SparseArray &&other) noexcept;

            reference_type operator[](size_t idx);
            const_reference_type operator[](size_t idx) const;

            decltype(auto) begin();
            decltype(auto) begin() const;
            decltype(auto) cbegin() const;

            decltype(auto) end();
            decltype(auto) end() const;
            decltype(auto) cend() const;

            size_type size() const;
            reference_type insert_at(size_type pos, TComponent const &component);
            reference_type insert_at(size_type pos, TComponent &&component);
            template <class... Params>
            reference_type emplace_at(size_type pos, Params &&...);
            void erase(size_type pos);
            size_type get_index(value_type const &) const;

            container_t &GetContainer();
            const container_t &GetContainer() const;

        private:
            container_t container;
    };

    template<typename TComponent>
    SparseArray<TComponent>::SparseArray(SparseArray &&other) noexcept : container(std::move(other.GetContainer()))
    {
    }

    template<typename TComponent>
    SparseArray<TComponent> & SparseArray<TComponent>::operator=(SparseArray &&other) noexcept
    {
        if (std::addressof(other) == this)
            return *this;

        GetContainer() = std::move(other.GetContainer());
        return *this;
    }

    template<typename TComponent>
    typename SparseArray<TComponent>::reference_type SparseArray<TComponent>::operator[](size_t idx)
    {
        if (idx >= size())
            GetContainer().resize(idx + 1, std::nullopt);
        return GetContainer()[idx];
    }

    template<typename TComponent>
    typename SparseArray<TComponent>::const_reference_type SparseArray<TComponent>::operator[](size_t idx) const
    {
        return GetContainer()[idx];
    }

    template<typename TComponent>
    decltype(auto) SparseArray<TComponent>::begin()
    {
        return GetContainer().begin();
    }

    template<typename TComponent>
    decltype(auto) SparseArray<TComponent>::begin() const
    {
        return GetContainer().begin();
    }

    template<typename TComponent>
    decltype(auto) SparseArray<TComponent>::cbegin() const
    {
        return GetContainer().cbegin();
    }

    template<typename TComponent>
    decltype(auto) SparseArray<TComponent>::end()
    {
        return GetContainer().end();
    }

    template<typename TComponent>
    decltype(auto) SparseArray<TComponent>::end() const
    {
        return GetContainer().end();
    }

    template<typename TComponent>
    decltype(auto) SparseArray<TComponent>::cend() const
    {
        return GetContainer().cend();
    }

    template<typename TComponent>
    typename SparseArray<TComponent>::size_type SparseArray<TComponent>::size() const
    {
        return GetContainer().size();
    }

    template<typename TComponent>
    template<class ... TParams>
    typename SparseArray<TComponent>::reference_type SparseArray<TComponent>::emplace_at(size_type pos, TParams &&...args)
    {
        if (GetContainer().size() <= pos)
            GetContainer().resize(pos + 1, std::nullopt);

        auto& opt = *GetContainer().emplace(GetContainer().begin() + pos);
        opt.emplace(std::forward<TParams>(args)...);
        return opt;
    }

    template<typename TComponent>
    void SparseArray<TComponent>::erase(size_type pos)
    {
        GetContainer()[pos].reset();
    }

    template<typename TComponent>
    typename SparseArray<TComponent>::container_t & SparseArray<TComponent>::GetContainer()
    {
        return container;
    }

    template<typename TComponent>
    const typename SparseArray<TComponent>::container_t & SparseArray<TComponent>::GetContainer() const
    {
        return container;
    }
} // MGE

#endif //SPARSEARRAY_HPP
