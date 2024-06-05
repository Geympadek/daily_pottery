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

    constexpr int MIN_PIXEL_WIDTH = 192;
    constexpr int MIN_PIXEL_HEIGHT = 108;

    extern Rect gScreen;

    class Window
    {
    public:
        Window(const char* title, int width, int height) {init(title, width, height);}
        Window(const std::string& title, int width, int height) {init(title, width, height);}
        ~Window();

        void setTitle(const char* title) noexcept;
        void setTitle(const std::string& title) noexcept {setTitle(title.c_str());}

        void setResolution(int width, int height) noexcept;
        
        void updatePixelScale() const noexcept;
        void update() noexcept;
        void updateScreen() noexcept;

        void fill(Color color) noexcept;
    private:
        void init(const std::string& title, int width, int height);
    private:
        SDL_Window* window;
        int width;
        int height;
        std::string title;
    };
}