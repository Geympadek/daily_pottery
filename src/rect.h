#pragma once

#include "vector2.h"
#include <SDL.h>

namespace engix
{
    struct Rect
    {
        Vector2i start;
        size_t width;
        size_t height;

        constexpr Rect(int x, int y, size_t width, size_t height) noexcept:
            start(x, y), width(width), height(height) {}
        constexpr Rect(Vector2i start = {0, 0}, size_t width = 0u, size_t height = 0u) noexcept :
            start(start), width(width), height(height) {}
        constexpr operator SDL_Rect() const noexcept {return {start.x, start.y, static_cast<int>(width), static_cast<int>(height)};}

        operator json::Value() const noexcept {return toJson();}

        json::Value toJson() const noexcept
        {
            json::Value json;
            json["start"] = start;
            json["width"] = width;
            json["height"] = height;
            return json;
        }
    };
}