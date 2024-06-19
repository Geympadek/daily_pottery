#pragma once

#include "vector2.h"
#include <SDL.h>
#include "math.h"

namespace engix
{
    struct Rect
    {
        Vector2i start;
        int width = 0;
        int height = 0;

        constexpr Rect() noexcept {}
        constexpr Rect(int x, int y, int width, int height) noexcept:
            start(x, y), width(width), height(height) {}
        constexpr Rect(Vector2i start, int width, int height) noexcept :
            start(start), width(width), height(height) {}
        constexpr Rect(int width, int height) noexcept :
            start(), width(width), height(height) {}
        constexpr Rect(Vector2i a, Vector2i b) noexcept :
            start(std::min(a.x, b.x), std::min(a.y, b.y)), width(abs(a.x - b.x)), height(abs(a.y - b.y)) {}

        constexpr bool isAbove(Vector2i point) const noexcept
        {
            return start.x <= point.x && start.x + static_cast<int>(width) >= point.x &&
                start.y <= point.y && start.y + static_cast<int>(height) >= point.y;
        }
        
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
            result.width = json.get("width", 0).as<int>();
            result.height = json.get("height", 0).as<int>();
            return result;
        }
    };
}