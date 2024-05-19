#pragma once

#include <string>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include "rect.h"

#include "color.h"

namespace engix 
{
    extern SDL_Renderer* gRenderer;
    extern double pixelScale;
    extern double roundPixelScale;

    constexpr size_t MIN_PIXEL_WIDTH = 192;
    constexpr size_t MIN_PIXEL_HEIGHT = 108;

    extern Rect gScreen;

    class Window
    {
    public:
        Window(const char* title, size_t width, size_t height) {init(title, width, height);}
        Window(std::string title, size_t width, size_t height) {init(std::move(title), width, height);}
        ~Window();

        void setTitle(const char* title) noexcept;
        void setTitle(const std::string& title) noexcept {setTitle(title.c_str());}

        void setResolution(size_t width, size_t height) noexcept;
        
        void updatePixelScale() const noexcept;
        void update() noexcept;
        void updateScreen() noexcept;

        void fill(Color color) noexcept;
    private:
        void init(std::string title, size_t width, size_t height);
    private:
        SDL_Window* window;
        size_t width;
        size_t height;
        std::string title;
    };
}