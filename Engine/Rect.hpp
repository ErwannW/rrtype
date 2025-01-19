//
// Created by nperreau on 2/1/24.
//

#ifndef MYLIB_RECT_HPP
#define MYLIB_RECT_HPP

#include <ostream>
#include "Vector/Vector2.hpp"

namespace MGE
{
    template<typename T>
    struct Rect {
        constexpr Rect() noexcept = default;
        inline constexpr Rect(T x, T y, T width, T height) noexcept : x(x), y(y), width(width), height(height) {}
        T x = 0;
        T y = 0;
        T width = 0;
        T height = 0;

        [[nodiscard]] inline constexpr Vector2<T> BottomRight() const noexcept
        {
            return { x + width, y + height };
        }

        Rect<T> &operator=(Rect<T> const &other);

        [[nodiscard]] inline constexpr bool Contains(Vector2<T> const &point) const noexcept {
            return point.x >= x && point.x <= x + width
                && point.y >= y && point.y <= y + height;
        }

        [[nodiscard]] inline constexpr Vector2<T> GetOffsetOf(Vector2<T> const &point) const noexcept {
            return point - GetPosition();
        }

        [[nodiscard]] inline constexpr Vector2<T> GetCenter() const noexcept {
            return { x + width / 2, y + height / 2 };
        }

        [[nodiscard]] inline constexpr Vector2<T> GetPosition() const noexcept { return {x, y }; }
        [[nodiscard]] inline constexpr Vector2<T> GetSize() const noexcept { return { width, height }; }

        [[nodiscard]] inline static constexpr bool Intersects(Rect<T> const &a, Rect<T> const &b) noexcept
        {
            // if rectangle has area 0, no overlap
            if (a.x == a.BottomLeft().x || a.y == a.BottomLeft().y || b.BottomRight().x == b.x || b.y == b.BottomRight().y)
                return false;

            // If one rectangle is on left side of other
            if (a.x > b.BottomRight().x || b.x > a.BottomLeft().x)
                return false;

            // If one rectangle is above other
            if (a.BottomLeft().y > b.y || b.BottomRight().y > a.y)
                return false;

            return true;
        }
    };

    template <typename T>
    inline Rect<T> &Rect<T>::operator=(Rect<T> const &other)
    {
        if (std::addressof(other) == this)
            return *this;

        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;
        return *this;
    }

    template<typename T>
    std::ostream &operator <<(std::ostream &stream, Rect<T> const &rect)
    {
        stream << '{' << rect.x << ',' << rect.y << ',' << rect.width << ',' << rect.height << '}';
        return stream;
    }
}

#endif //MYLIB_RECT_HPP