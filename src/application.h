#pragma once

#include <SDL_image.h>
#include <SDL_mixer.h>
#include "contentWrap.h"
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

namespace engix
{
    using std::unordered_map;
    using std::string;
    using std::vector;
    using std::array;
    using std::shared_ptr;
    using std::unique_ptr;
    using std::static_pointer_cast;
    using std::dynamic_pointer_cast;

    // fist bit is responsible for state (either up or down)
    // second bit active only on the first time
    enum KeyState : uint8_t {FREE = 0b00, PRESS = 0b01, RELEASE = 0b10, FIRST = 0b11};

    const unsigned NUMBER_OF_KEYS = 4u;
    enum Key {UP, LEFT, DOWN, RIGHT};

    const std::array<int, NUMBER_OF_KEYS> controls
    {
        SDL_SCANCODE_W,
        SDL_SCANCODE_A,
        SDL_SCANCODE_S,
        SDL_SCANCODE_D
    };

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
        void handleMouseDown(SDL_MouseButtonEvent e);
        void handleMouseUp(SDL_MouseButtonEvent e);
        void handleEvents();
    public:
        int indexFromScancode(SDL_Scancode scancode);
    private:
        bool isRunning = true;

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

        array<KeyState, NUMBER_OF_KEYS> keyboardState;

        unordered_map<string, shared_ptr<Texture>> textures;

        Font pixixFont;

        BoxTemplate backgroundTemplate;
    };
}