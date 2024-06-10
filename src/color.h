#pragma once

#include <SDL.h>
#include <string>
#include <sstream>
#include <cstring>

namespace engix
{
    struct Color
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;

        constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) noexcept : red(red), green(green), blue(blue), alpha(alpha) {}
        
        constexpr Color(uint32_t hex = 0x00000000) noexcept { loadFromHex(hex); }
        // Color(const Json::Value& json) { loadFromJson(json); }
        Color(const std::string& str) noexcept { loadFromString(str); }
        Color(const char* str) noexcept {loadFromString(str);}

        constexpr void loadFromHex(uint32_t color) noexcept;
        void loadFromString(const std::string& str) noexcept {loadFromString(str.c_str());}
        void loadFromString(const char* str) noexcept;
        // void loadFromJson(const Json::Value& json);

        constexpr uint32_t hex() const noexcept {return (red << 24) | (green << 16) | (blue << 8) | alpha;}

        constexpr bool operator==(const Color& color) const noexcept {return red == color.red && green == color.green && blue == color.blue && alpha == color.alpha;}
        constexpr bool operator!=(const Color& color) const noexcept {return !operator==(color);}
        constexpr operator SDL_Color() const noexcept {return {red, green, blue, alpha};};
        
        static const Color WHITE;
        static const Color BLACK;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color CYAN;
        static const Color TRANSPARENT;
    };
}