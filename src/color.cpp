#include "color.h"

using namespace engix;

const Color Color::WHITE(255, 255, 255);
const Color Color::BLACK(0, 0, 0);
const Color Color::RED(255, 0, 0);
const Color Color::GREEN(0, 255, 0);
const Color Color::BLUE(0, 0, 255);
const Color Color::CYAN(0, 255, 255);
const Color Color::TRANSPARENT(0, 0, 0, 0);

constexpr void engix::Color::loadFromHex(uint32_t color) noexcept
{
    red = color >> 24;
    green = color >> 16;
    blue = color >> 8;
    alpha = color;
}

void engix::Color::loadFromString(const char *str) noexcept
{
    if (str[0] == '#')
    {
        str++;
    }
    std::stringstream ss;
    ss << std::hex << str;
    uint32_t color;
    ss >> color;
    switch (std::strlen(str))
    {
    case 6:
        loadFromHex((color << 8u) | 0xffu);
        break;
    case 4:
        loadFromHex((color << 16u) | (color << 8u) | 0xffu);
        break;
    case 2:
        loadFromHex((color << 24u) | (color << 16u) | (color << 8u) | 0xffu);
        break;
    default:
        loadFromHex(color);
        break;
    }
}

// void engine::Color::loadFromJson(const Json::Value &json)
// {
//     loadFromString(json.asCString());
// }
