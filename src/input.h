#pragma once

#include <SDL.h>
#include <SDL_keyboard.h>

#include <string>
#include <vector>

#include "vector2.h"

using std::u16string;

namespace engix
{
    struct Input
    {
        struct Cursor
        {
            enum State : uint8_t {NONE = 0b0, LEFT = 0b1, MIDDLE = 0b10, RIGHT = 0b100};

            Vector2i position;
            Vector2i movement;
            uint8_t state = 0;
            bool isShown = true;
        } cursor;

        void update();
    private:
        void handleEvents();
        void handleKeyboard();
        void handleMouseMovement();
        void handleWindowEvent(const SDL_WindowEvent& e);
        void handleMouseDown(const SDL_MouseButtonEvent& e);
        void handleMouseUp(const SDL_MouseButtonEvent& e);
        void handleTextInput(const SDL_TextInputEvent& e);
        void onKeyDown(const SDL_KeyboardEvent& e);
        void onKeyUp(const SDL_KeyboardEvent& e);
    public:
        u16string text;
        std::vector<size_t> selection;
        int textCursor = 0;
        bool isReading = false;
        bool lastReading = false;
        bool isRunning = true;
        bool shift = false;
        bool ctrl = false;
    };
}