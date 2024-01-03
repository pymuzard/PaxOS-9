//
// Created by Charlito33 on 26/12/2023.
// Copyright (c) 2023 Charlito33. All rights reserved.
//

#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <cstdint>

#include "LovyanGFX.hpp"

namespace graphics
{
    class Surface
    {
        friend void renderSurface(const Surface *surface);

    private:
        lgfx::LGFX_Sprite m_sprite;

    public:
        Surface(uint16_t width, uint16_t height);
        ~Surface();

        [[nodiscard]] uint16_t getWidth() const;
        [[nodiscard]] uint16_t getHeight() const;

        void pushSurface(Surface *surface, int16_t x, int16_t y);

        void clear(uint8_t r, uint8_t g, uint8_t b);
        void clear();

        void setColor(uint8_t r, uint8_t g, uint8_t b);

        void fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
        void drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h);

        void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    };
} // graphics

#endif //SURFACE_HPP
