#pragma once

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include <memory>
#include "assets.h"
#include "random.h"
#include "clock.h"
#include <array>
#include "font.h"
#include "timer.h"
#include "button.h"
#include "boxTemplate.h"
#include "slider.h"
#include "textBox.h"
#include "inputBox.h"
#include "input.h"
#include "encoding.h"

using std::unordered_map;
using std::string;
using std::vector;
using std::array;
using std::shared_ptr;
using std::unique_ptr;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

namespace engix
{
    class Application
    {
    public:
        Application();
        ~Application();

        void loadAssets();

        void loop();

        void update();
        void render();
        void handleInput();
    private:
        void handleKeyboard();
        void handleMouseMovement();
        void handleMouseDown(SDL_MouseButtonEvent e);
        void handleMouseUp(SDL_MouseButtonEvent e);
        void handleEvents();
        void onKeyDown(const SDL_Event& event);
    public:
        // int indexFromScancode(SDL_Scancode scancode);
    private:
        bool isRunning = true;

        Input input;

        Clock delta;
        Window window;
        Mouse mouse;

        Timer logoTimer;

        enum Page : uint8_t {
            LOGO,
            SEARCH,
            ADD,
            TEMPER,
            MAIN_INFO,
            THICKNESS,
            DRYING,
            FIREING,
            SUCCESS,
            NUMBER_OF_PAGES
        };

        Page currentPage;

        array<vector<std::shared_ptr<VisualElement>>, NUMBER_OF_PAGES> pages;

        unordered_map<string, shared_ptr<Texture>> textures;
        unordered_map<string, shared_ptr<BoxTemplate>> templates;
        unordered_map<string, shared_ptr<Font>> fonts;
    };
}