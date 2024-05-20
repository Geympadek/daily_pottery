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
        constexpr Rect(Vector2i start, size_t width, size_t height) noexcept :
            start(start), width(width), height(height) {}
        constexpr Rect(size_t width, size_t height) noexcept :
            start(), width(width), height(height) {}
        constexpr Rect() noexcept :
            start(), width(0), height(0) {}
        
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

        static Rect fromJson(const json::Value& json) noexcept
        {
            Rect result;
            result.start = Vector2i::fromJson(json["start"]);
            result.width = json.get("width", 0).as<size_t>();
            result.height = json.get("height", 0).as<size_t>();
            return result;
        }
    };
}