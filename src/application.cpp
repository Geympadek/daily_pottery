#include "application.h"

engix::Application::Application() 
: window("Pottery!", 1280, 720), keyboardState {KeyState::FREE}, logoTimer(000)
{
    currentPage = Page::LOGO;

    loadAssets();

    mouse = Mouse(textures["cursor"]);

    auto doNothing = [](VisualElement*, const Mouse&){};

    auto background = std::make_shared<VisualElement>(textures["background"], Vector2i{0, 0});
    background->scaling(Texture::Scaling::NONE);
    for (auto& page : pages)
    {
        page.push_back(background);
    }
    
    pages[Page::LOGO].push_back(std::make_shared<VisualElement>(textures["logo"], Vector2i{0, 0}, MIN_PIXEL_WIDTH, MIN_PIXEL_HEIGHT));
    pages[Page::LOGO].back()->scaling(Texture::Scaling::STRETCH);
    pages[Page::SEARCH].push_back(std::make_shared<Button>(textures["searchButton"], Vector2i{2, 3}));
    pages[Page::SEARCH].back()->horizontalAlign(engix::Align::END);

    pages[Page::SEARCH].push_back(std::make_shared<VisualElement>(textures["exit"], Vector2i{0, 0}));
    pages[Page::SEARCH].back()->horizontalRepeat(engix::Repeat::REPEAT);
    pages[Page::SEARCH].back()->verticalRepeat(engix::Repeat::REPEAT);

    Clock clock;
    pages[Page::SEARCH].push_back(
        std::make_shared<VisualElement>(
            std::make_shared<Texture>(hoverBox.genTexture(120, 100)),
            Vector2i{}
            )
        );
    std::cout << clock.millis() << '\n';
    pages[Page::SEARCH].back()->horizontalAlign(engix::Align::CENTER);
    pages[Page::SEARCH].back()->verticalAlign(engix::Align::CENTER);

    logoTimer.start();
}

engix::Application::~Application()
{
}

void engix::Application::loadAssets()
{
    pixixFont = Font::load(assets::FONTS_PATH + "pixix");

    textures["cursor"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "cursor.png");
    textures["logo"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "logo.png");
    textures["background"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "background.png");
    textures["searchButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "searchButton.png");
    textures["exit"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "exit.png");
    textures["buttonTemplate"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "buttonTemplate.png");

    textures["exitCopy"] = std::make_shared<Texture>(*textures["exit"]);

    hoverBox = BoxTemplate::load(assets::TEXTURES_PATH + "buttonTemplate");
}

void engix::Application::loop()
{
    while (isRunning)
    {
        handleInput();
        update();
        render();
    }
}

void engix::Application::update()
{
    delta.restart();

    logoTimer.update();
    if (currentPage == Page::LOGO && !logoTimer.isRunning())
    {
        currentPage = Page::SEARCH;
    }

    for (const auto& page : pages[currentPage])
    {
        page->update(mouse);
    }
    mouse.update();
}

void engix::Application::render()
{
    window.fill({"8a4009"});

    for (const auto& page : pages[currentPage])
    {
        page->render();
    }
    textures["exitCopy"]->render({10, 15});

    mouse.render();
    window.update();
}

void engix::Application::handleInput()
{
    handleEvents();
    handleKeyboard();
}

void engix::Application::handleKeyboard()
{
    auto currentKeyboardState = SDL_GetKeyboardState(nullptr);
    
    for (unsigned i = 0; i < NUMBER_OF_KEYS; i++)
    {
        if (currentKeyboardState[controls[i]])
        {
            switch (keyboardState[i])
            {
            case KeyState::FREE:
            case KeyState::RELEASE:
                keyboardState[i] = KeyState::FIRST;
                break;
            case KeyState::FIRST:
                keyboardState[i] = KeyState::PRESS;
                break;
            }
        }
        else
        {
            switch (keyboardState[i])
            {
            case KeyState::FIRST:
            case KeyState::PRESS:
                keyboardState[i] = KeyState::RELEASE;
                break;
            case KeyState::RELEASE:
                keyboardState[i] = KeyState::FREE;
                break;
            }
        }
    }
}

void engix::Application::handleMouseDown(SDL_MouseButtonEvent e)
{
    switch (e.button)
    {
    case SDL_BUTTON_LEFT:
        mouse.state(mouse.state() | Mouse::State::LEFT);
        break;
    }
}

void engix::Application::handleMouseUp(SDL_MouseButtonEvent e)
{
    switch (e.button)
    {
    case SDL_BUTTON_LEFT:
        mouse.state(mouse.state() ^ Mouse::State::LEFT);
        break;
    }
}

void engix::Application::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_ENTER:
                mouse.isShown(true);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                mouse.isShown(false);
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouseDown(e.button);
            break;
        case SDL_MOUSEBUTTONUP:
            handleMouseUp(e.button);
            break;
        }
    }
}

int engix::Application::indexFromScancode(SDL_Scancode scancode)
{
    switch (scancode)
    {
    case SDL_SCANCODE_W:
        return Key::UP;
    case SDL_SCANCODE_A:
        return Key::LEFT;
    case SDL_SCANCODE_S:
        return Key::DOWN;
    case SDL_SCANCODE_D:
        return Key::RIGHT;
    default:
        return -1;
    }
}
