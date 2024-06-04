#pragma once

#include <SDL.h>
#include <SDL_keyboard.h>

#include <array>
#include <string>
#include <vector>

using std::u16string;
using std::array;

namespace engix
{
    struct Input
    {
        u16string text;
        std::vector<size_t> selection;
        int textCursor = 0;
        bool isReading = false;

        struct Cursor
        {
            enum State : uint8_t {NONE = 0b0, LEFT = 0b1, MIDDLE = 0b10, RIGHT = 0b100};

            Vector2i position;
            Vector2i movement;
            uint8_t state = 0;
            bool isShown = true;
        } cursor;
    };
}