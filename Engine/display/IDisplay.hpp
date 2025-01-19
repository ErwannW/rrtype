/*
** EPITECH PROJECT, 2024
** display
** File description:
** IDisplay
*/

#ifndef IDISPLAY_HPP_
#define IDISPLAY_HPP_

#include <utility>
#include "Rendering/IRenderer.hpp"

class IDisplay : public IRenderer
{
    public:
        virtual void InitDisplay() = 0;

        virtual std::pair<float, float> getMousePosInPercent() = 0;
        virtual bool IsMouseLeftPressed() = 0;
        virtual bool IsMouseRightPressed() = 0;
};

#endif /* !IDISPLAY_HPP_ */
