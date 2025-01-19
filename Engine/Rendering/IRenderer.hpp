//
// Created by Nail on 20/11/2024.
//

#ifndef IRENDERER_HPP
#define IRENDERER_HPP

/**
 * Interface for rendering.
 */
class IRenderer
{
    public:
        virtual ~IRenderer() = default;

        /**
         * Called before drawing the frame.
         */
        virtual void PreDraw() = 0;

        /**
         * Called after all entities of this frame has been drawn.
         */
        virtual void PostDraw() = 0;
};

#endif //IRENDERER_HPP
