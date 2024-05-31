#pragma once

#include <SDL.h>
#include <SDL_keyboard.h>

#include <array>
#include <string>

using std::u16string;
using std::array;

namespace engix
{
    // fist bit is responsible for state (either up or down)
    // second bit active only on the first time
    // enum KeyState : uint8_t {FREE = 0b00, PRESS = 0b01, RELEASE = 0b10, FIRST = 0b11};
    
    // const unsigned NUMBER_OF_KEYS = 4u;
    // enum Key {UP, LEFT, DOWN, RIGHT};
    // constexpr std::array<int, NUMBER_OF_KEYS> controls
    // {
    //     SDL_SCANCODE_W,
    //     SDL_SCANCODE_A,
    //     SDL_SCANCODE_S,
    //     SDL_SCANCODE_D
    // };

    struct Input
    {
        u16string text;
        int cursor = 0;
        bool isReading = false;
        // array<KeyState, NUMBER_OF_KEYS> keyboardState;

        // static SDL_Scancode scancodeFromKey(Key key)
        // {
        //     return static_cast<SDL_Scancode>(key);
        // }
    };
}