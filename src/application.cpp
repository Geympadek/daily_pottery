#include "application.h"

engix::Application::Application() 
: window("Pottery!", 1280, 720), logoTimer(000)
{
    currentPage = Page::LOGO;

    loadAssets();

    mouse = Mouse(textures["cursor"]);

    auto doNothing = [](VisualElement*, const Mouse&){};

    auto background = std::make_shared<VisualElement>(std::make_shared<Texture>(templates["background"]->genTexture(gScreen.width, gScreen.height)));
    background->scaling(Texture::Scaling::NONE);
    for (auto& page : pages)
    {
        page.push_back(background);
    }
    
    pages[Page::LOGO].push_back(std::make_shared<VisualElement>(textures["logo"], gScreen.width, gScreen.height));
    pages[Page::LOGO].back()->scaling(Texture::Scaling::STRETCH);
    
    pages[Page::SEARCH].push_back(std::make_shared<Button>(textures["searchButton"]));
    pages[Page::SEARCH].back()->relativePos({3, 2});
    pages[Page::SEARCH].back()->horizontalAlign(engix::Align::END);

    auto textField = std::make_shared<InputBox>(templates["textField"]);
    textField->font(fonts["default"]);
    textField->horizontalAlign(Align::CENTER);
    textField->verticalAlign(Align::CENTER);
    textField->padding(2, 1, 2, 1);
    textField->width(60);
    textField->height(10);

    pages[Page::SEARCH].push_back(std::move(textField));

    logoTimer.start();
}

engix::Application::~Application()
{
}

void engix::Application::loadAssets()
{
    fonts["default"] = std::make_shared<Font>(assets::FONTS_PATH + "pixix");

    textures["cursor"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "cursor.png");
    textures["logo"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "logo.png");
    textures["searchButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "searchButton.png");
    textures["verticalSliderPoint"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "verticalSliderPoint.png");

    templates["background"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "backgroundTemplate");
    templates["bigTextField"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "bigTextFieldTemplate");
    templates["label"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "labelTemplate");
    templates["sliderLine"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "sliderLineTemplate");
    templates["textField"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "textFieldTemplate");
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
    
    // for (unsigned i = 0; i < NUMBER_OF_KEYS; i++)
    // {
    //     if (currentKeyboardState[controls[i]])
    //     {
    //         switch (keyboardState[i])
    //         {
    //         case KeyState::FREE:
    //         case KeyState::RELEASE:
    //             keyboardState[i] = KeyState::FIRST;
    //             break;
    //         case KeyState::FIRST:
    //             keyboardState[i] = KeyState::PRESS;
    //             break;
    //         }
    //     }
    //     else
    //     {
    //         switch (keyboardState[i])
    //         {
    //         case KeyState::FIRST:
    //         case KeyState::PRESS:
    //             keyboardState[i] = KeyState::RELEASE;
    //             break;
    //         case KeyState::RELEASE:
    //             keyboardState[i] = KeyState::FREE;
    //             break;
    //         }
    //     }
    // }
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

// int engix::Application::indexFromScancode(SDL_Scancode scancode)
// {
//     switch (scancode)
//     {
//     case SDL_SCANCODE_W:
//         return Key::UP;
//     case SDL_SCANCODE_A:
//         return Key::LEFT;
//     case SDL_SCANCODE_S:
//         return Key::DOWN;
//     case SDL_SCANCODE_D:
//         return Key::RIGHT;
//     default:
//         return -1;
//     }
// }
