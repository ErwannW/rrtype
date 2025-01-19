//
// Created by nperreau on 2/1/24.
//

#ifndef MYLIB_VECTOR2_HPP
#define MYLIB_VECTOR2_HPP

#include <complex>
#include <iostream>
#include "Types.hpp"

namespace MGE
{
    /// @brief A two component vector.
    template<typename T>
    struct Vector2
    {
        /// @brief Construct a new vector with all its components initialized to 0.
        constexpr Vector2() = default;
        /// @brief Construct a new vector with the given values.
        constexpr Vector2(T x, T y) : x(x), y(y) {};
        /// @brief Construct a new vector with all its components initialized to value.
        /// @param value The value of the x and y components.
        constexpr Vector2(T value) : x(value), y(value) {}; // NOLINT(*-explicit-constructor)
        /// @brief Construct a new vector by copy.
        template<typename TOther>
        constexpr Vector2(Vector2<TOther> const &other) : x(other.x), y(other.y) {} // NOLINT(*-explicit-constructor)

        T x = T(0);
        T y = T(0);

        static inline const Vector2<T> Zero = 0;
        static inline const Vector2<T> One = 1;

        /// @brief Creates a new vector with the given x value.
        /// @returns A copy of this vector with the given x value.
        [[nodiscard]] constexpr Vector2<T> WithX(T x) const noexcept;
        /// @brief Creates a new vector with the given y value.
        /// @returns A copy of this vector with the given y value.
        [[nodiscard]] constexpr Vector2<T> WithY(T y) const noexcept;

        /// @brief Converts this vector to a vector of another type.
        /// @returns A copy of this vector of type TOther.
        /// @tparam TOther The type of the new vector.
        template<typename TOther>
        [[nodiscard]] constexpr Vector2<TOther> To() const noexcept;

        /// @brief Checks if two vectors are equals.
        /// @details Two vectors are considered equal when its x and y values are equal.
        /// @returns true if two vectors are equals, false otherwise.
        template<typename TOther>
        [[nodiscard]] constexpr bool operator ==(Vector2<TOther> const &other) const {
            return x == other.x
                && y == other.y;
        }

        /// @brief Checks if all components of this vector are equal to val.
        /// @returns true if all components of this vector are equal to val, false otherwise.
        [[nodiscard]] constexpr bool operator ==(T const val) const {
            return x == val
                && y == val;
        }

        /// @brief Copy operator.
        template<typename TOther>
        constexpr Vector2<T> &operator =(Vector2<TOther> const &other) noexcept {
            x = other.x;
            y = other.y;
            return *this;
        }

        /// @brief Checks if two vectors are not equal.
        /// @details Two vectors are considered not equal when its x or y values aren't equal.
        /// @returns true if two vectors aren't equals, false otherwise.
        template<typename TOther>
        [[nodiscard]] constexpr bool operator !=(Vector2<TOther> const &other) const {
            return x != other.x
                || y != other.y;
        }

        /// @brief Checks if all components of this vector are not equal to val.
        /// @returns true if all components of this vector are not equal to val, false otherwise.
        [[nodiscard]] constexpr bool operator !=(T const val) const {
            return x != val
                || y != val;
        }

        [[nodiscard]] constexpr Vector2<T> operator-(Vector2<T> const &other) const noexcept {
            return { x - other.x, y - other.y };
        }

        [[nodiscard]] constexpr Vector2<T> operator+(Vector2<T> const &other) const noexcept {
            return { x + other.x, y + other.y };
        }

        [[nodiscard]] constexpr Vector2<T> operator*(T const nb) const noexcept {
            return { x * nb, y * nb };
        }

        [[nodiscard]] constexpr Vector2<T> operator/(T const nb) const noexcept {
            return { x / nb, y / nb };
        }

        constexpr Vector2<T> operator+=(Vector2<T> const &other) noexcept {
            x += other.x;
            y += other.y;
            return *this;
        }

        /// @brief GetValue the length of this vector.
        /// @returns The length of this vector.
        [[nodiscard]] constexpr double Length() const noexcept;
        /// @brief GetValue the square root length of this vector.
        /// @returns The square root length of this vector.
        [[nodiscard]] constexpr double SqrLength() const noexcept;
    };

    template<typename T>
    std::ostream &operator <<(std::ostream &stream, Vector2<T> const &vec)
    {
        return stream << "{x:" << vec.x << ",y:" << vec.y << "}";
    }

    template <typename T>
    template <typename TOther>
    constexpr Vector2<TOther> Vector2<T>::To() const noexcept
    {
        return Vector2<TOther>(TOther(x), TOther(y));
    }

    template<typename T>
    constexpr double Vector2<T>::SqrLength() const noexcept
    {
        return x * x + y * y;
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::WithX(T const x) const noexcept
    {
        return { x, y };
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::WithY(T const y) const noexcept
    {
        return { x, y };
    }

    template <typename T>
    constexpr double Vector2<T>::Length() const noexcept
    {
        return std::sqrt(SqrLength());
    }

    using Vector2f = Vector2<float64>;
    using Vector2i = Vector2<int32>;
}
#endif //MYLIB_VECTOR2_HPP