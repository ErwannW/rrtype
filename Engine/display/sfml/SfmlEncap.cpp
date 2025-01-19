/*
** EPITECH PROJECT, 2024
** display
** File description:
** SfmlEncap
*/

#include "SfmlDisplay.hpp"
#include "../IDisplay.hpp"

extern "C" {
    IDisplay *createDisplay() {
        return new SfmlDisplay();
    }
}