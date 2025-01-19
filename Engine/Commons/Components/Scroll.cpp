/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Scroll
*/

#include "Scroll.hpp"

namespace MGE
{
    void ScrollComponent::SetupReflection()
    {
        Class.RegisterProperty(scroll, REPLICATED);
        Class.RegisterProperty(hasDuplicated, REPLICATED);
        Class.RegisterProperty(scrollSpeed, REPLICATED);

        Class.RegisterProperty(id, REPLICATED);
    }
}
