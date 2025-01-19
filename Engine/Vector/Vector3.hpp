//
// Created by nperreau on 3/1/34.
//

#ifndef MYLIB_VECTOR3_HPP
#define MYLIB_VECTOR3_HPP

#include <complex>
#include <iostream>

#include "Math.hpp"
#include "Types.hpp"

namespace MGE
{
    /// @brief A three component vector.
    template<typename T>
    struct Vector3
    {
        /// @brief Construct a new vector with all its components initialized to 0.
        constexpr Vector3() = default;
        /// @brief Construct a new vector with the given values.
        constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {};
        /// @brief Construct a new vector with all its components initialized to value.
        /// @param value The value of the x and y components.
        constexpr Vector3(T value) : x(value), y(value), z(value) {}; // NOLINT(*-explicit-constructor)
        /// @brief Construct a new vector by copy.
        template<typename TOther>
        constexpr Vector3(Vector3<TOther> const &other) : x(other.x), y(other.y), z(other.z) {} // NOLINT(*-explicit-constructor)

        T x = T(0);
        T y = T(0);
        T z = T(0);

        [[nodiscard]] constexpr Vector3<T> operator *(Vector3<T> const &other) const noexcept
        {
            return { x * other.x, y * other.y, z * other.z };
        }

        [[nodiscard]] constexpr Vector3<T> operator /(Vector3<T> const &other) const noexcept
        {
            return { x / other.x, y / other.y, z / other.z };
        }

        /// @brief Creates a new vector with the given x value.
        /// @returns A copy of this vector with the given x value.
        [[nodiscard]] constexpr Vector3<T> WithX(T x) const noexcept;
        /// @brief Creates a new vector with the given y value.
        /// @returns A copy of this vector with the given y value.
        [[nodiscard]] constexpr Vector3<T> WithY(T y) const noexcept;
        /// @brief Creates a new vector with the given z value.
        /// @returns A copy of this vector with the given z value.
        [[nodiscard]] constexpr Vector3<T> WithZ(T z) const noexcept;

        /// @brief GetValue the normalized vector of this vector.
        /// @details Changes the values to make its bigger 1 and smaller 0, while keeping the proportionality between the components.
        [[nodiscard]] constexpr Vector3<T> Normalized() const noexcept
        {
            return (*this) / static_cast<T>(Length());
        }

        /// @brief Converts this vector to a vector of another type.
        /// @returns A copy of this vector of type TOther.
        /// @tparam TOther The type of the new vector.
        template<typename TOther>
        [[nodiscard]] constexpr Vector3<TOther> To() const noexcept;

        template<typename TOther>
        [[nodiscard]] constexpr bool operator ==(Vector3<TOther> const &other) const {
            return x == other.x
                && y == other.y
                && z == other.z;
        }

        /// @brief Checks if two vectors are equals.
        /// @details Two vectors are considered equal when its x and y values are equal.
        /// @returns true if two vectors are equals, false otherwise.
        [[nodiscard]] constexpr bool operator ==(T const val) const {
            return x == val
                && y == val
                && z == val;
        }

        template<typename TOther>
        constexpr Vector3<T> &operator =(Vector3<TOther> const &other) noexcept {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        /// @brief Checks if two vectors are not equal.
        /// @details Two vectors are considered not equal when its x or y values aren't equal.
        /// @returns true if two vectors aren't equals, false otherwise.
        template<typename TOther>
        [[nodiscard]] constexpr bool operator !=(Vector3<TOther> const &other) const {
            return x != other.x
                || y != other.y
                || z != other.z;
        }

        /// @brief Checks if all components of this vector are not equal to val.
        /// @returns true if all components of this vector are not equal to val, false otherwise.
        [[nodiscard]] constexpr bool operator !=(T const val) const {
            return x != val
                || y != val
                || z != val;
        }

        [[nodiscard]] constexpr Vector3<T> operator-(Vector3<T> const &other) const noexcept {
            return { x - other.x, y - other.y, z - other.z };
        }

        [[nodiscard]] constexpr Vector3<T> operator-() const noexcept {
            return { -x, -y, -z };
        }

        [[nodiscard]] constexpr Vector3<T> operator+(Vector3<T> const &other) const noexcept {
            return { x + other.x, y + other.y, z + other.z };
        }

        [[nodiscard]] constexpr Vector3<T> operator*(T const nb) const noexcept {
            return { x * nb, y * nb, z * nb };
        }

        [[nodiscard]] constexpr Vector3<T> operator/(T const nb) const noexcept {
            return { x / nb, y / nb, z / nb };
        }

        constexpr Vector3<T> operator+=(Vector3<T> const &other) noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        /// @brief GetValue the vector rotated on its X axis.
        /// @returns A copy of this vector, rotated on its X axis.
        [[nodiscard]] constexpr Vector3<T> RotatedX(T degrees) const noexcept
        {
            Vector3<T> vec = *this;

            T radians = degrees * PI / 180.0;
            T cosTheta = std::cos(radians);
            T sinTheta = std::sin(radians);
            vec.y = y * cosTheta - z * sinTheta;
            vec.z = y * sinTheta + z * cosTheta;
            return vec;
        }

        /// @brief GetValue the vector rotated on its Y axis.
        /// @returns A copy of this vector, rotated on its Y axis.
        [[nodiscard]] constexpr Vector3<T> RotatedY(T degrees) const noexcept
        {
            Vector3<T> vec = *this;

            T radians = degrees * PI / 180.0;
            T cosTheta = std::cos(radians);
            T sinTheta = std::sin(radians);
            vec.x = x * cosTheta + z * sinTheta;
            vec.z = z * cosTheta - x * sinTheta;
            return vec;
        }

        /// @brief GetValue the vector rotated on its Z axis.
        /// @returns A copy of this vector, rotated on its Z axis.
        [[nodiscard]] constexpr Vector3<T> RotatedZ(T degrees) const noexcept
        {
            Vector3<T> vec = *this;

            T radians = degrees * PI / 180.0;
            T cosTheta = std::cos(radians);
            T sinTheta = std::sin(radians);
            vec.x = x * cosTheta - y * sinTheta;
            vec.y = x * sinTheta + y * cosTheta;
            return vec;
        }

        /// @brief GetValue the magnitude of this vector.
        /// @returns The magnitude of this vector.
        [[nodiscard]] constexpr double Length() const noexcept;
        /// @brief GetValue the squared magnitude of this vector.
        /// @returns The squared magnitude of this vector.
        [[nodiscard]] constexpr double SqrLength() const noexcept;
    };
    
    template<typename T>
    struct Direction
    {
        static constexpr const Vector3<T> Zero = 0;
        static constexpr const Vector3<T> One = 1;

        static constexpr const Vector3<T> Up = { 0, 1, 0 };
        static constexpr const Vector3<T> Down = -Up;

        static constexpr const Vector3<T> Left = { 0, 0, 1 };
        static constexpr const Vector3<T> Right = -Left;

        static constexpr const Vector3<T> Forward = { 1, 0, 0 };
        static constexpr const Vector3<T> Backward = -Forward;
    };

    template<typename T>
    std::ostream &operator <<(std::ostream &stream, Vector3<T> const &vec)
    {
        return stream << "{x:" << vec.x << ",y:" << vec.y <<  ",z:" << vec.z << "}";
    }

    template<typename T>
    template<typename TOther>
    constexpr Vector3<TOther> Vector3<T>::To() const noexcept
    {
        return Vector3<TOther>(TOther(x), TOther(y), TOther(z));
    }

    template<typename T>
    constexpr double Vector3<T>::SqrLength() const noexcept
    {
        return x * x + y * y + z * z;
    }

    template<typename T>
    constexpr Vector3<T> Vector3<T>::WithX(T const x) const noexcept
    {
        return { x, y, z };
    }

    template<typename T>
    constexpr Vector3<T> Vector3<T>::WithY(T const y) const noexcept
    {
        return { x, y, z };
    }

    template<typename T>
    constexpr Vector3<T> Vector3<T>::WithZ(T const z) const noexcept
    {
        return { x, y, z };
    }

    template<typename T>
    constexpr double Vector3<T>::Length() const noexcept
    {
        return std::sqrt(SqrLength());
    }

    /// @brief A three component vector of floats.
    using Vector3f = Vector3<float32>;
    /// @brief A three component vector of ints.
    using Vector3i = Vector3<int32>;

    using Direction3f = Direction<float32>;
    using Direction3i = Direction<int32>;
}
#endif //MYLIB_VECTOR3_HPP