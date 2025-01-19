/*
** EPITECH PROJECT, 2024
** display
** File description:
** main
*/

#include "IDisplay.hpp"
#include "./sfml/SfmlDisplay.hpp"
#include <iostream>
#include <dlfcn.h>


typedef IDisplay* (*CreateDisplayFunc)();

int main() {

    void *loadLib = dlopen("./Lib/SfmlLibDisplay.so", RTLD_LAZY);
    if (!loadLib) {
        std::cerr << "Error while loading" << dlerror() << std::endl;
        return 84;
    }

    CreateDisplayFunc createDisplay = (CreateDisplayFunc)dlsym(loadLib, "createDisplay");
    if (!createDisplay) {
        std::cerr << "Failed" << dlerror() << std::endl;
        dlclose(loadLib);
        return 84;
    }

    IDisplay *display = createDisplay();

    auto sfmlDisplay = dynamic_cast<SfmlDisplay*>(display);
    sf::Vector2f posPercent(30.0f, 40.0f);
    sf::Color color(0, 0, 255);
    sfmlDisplay->AddWinElem(std::make_shared<Square>(10.0f, posPercent, color, 40));

    sf::Vector2f posPercent4(30.0f, 40.0f);
    sf::Color color4(0, 255, 0);
    sfmlDisplay->AddWinElem(std::make_shared<Square>(10.0f, posPercent, color, 0));

    sf::Vector2f posPercent2(50.0f, 50.0f);
    sf::Color color2(255, 0, 0);
    sfmlDisplay->AddWinElem(std::make_shared<Rectangle>(20.0f, 10.0f, posPercent2, color2, 150));

    display->InitDisplay();
    display->Display();

    dlclose(loadLib);

    return 0;
}