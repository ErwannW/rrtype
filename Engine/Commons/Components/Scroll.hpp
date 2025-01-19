/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Scroll
*/

#ifndef SCROLL_HPP_
#define SCROLL_HPP_

#include "Core.hpp"

namespace MGE
{
    struct ScrollComponent
    {
        REFLECT_STRUCT(ScrollComponent)

        Vector3f scroll = 0;
        bool hasDuplicated = false;
        float scrollSpeed = 1.0f;

        //tmp
        int id = 0;

        explicit ScrollComponent(Vector3f scroll, int id, bool hasDuplicated = false, float scrollSpeed = 1.0f) : scroll(scroll) {}
    };
}

#endif /* !SCROLL_HPP_ */
