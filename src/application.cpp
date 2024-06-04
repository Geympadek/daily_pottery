#include "application.h"

using namespace engix;

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

    auto textField = std::make_shared<InputBox>(templates["textField"], textures["textCursor"]);
    textField->font(fonts["default"]);
    textField->horizontalAlign(Align::CENTER);
    textField->verticalAlign(Align::CENTER);
    textField->padding(2, 1, 2, 1);
    textField->textColor("3c1705");
    textField->minWidth(20);

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
    textures["textCursor"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "textCursor.png");

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

    bool prevReading = input.isReading;
    for (const auto& page : pages[currentPage])
    {
        if (page->enable())
            page->update(input);
    }
    if (mouse.enable())
        mouse.update(input);
    
    if (!prevReading && input.isReading)
    {
        SDL_StartTextInput();
    }
    else if (prevReading && !input.isReading)
    {
        SDL_StopTextInput();
    }
}

void engix::Application::render()
{
    window.fill({"8a4009"});

    for (const auto& page : pages[currentPage])
    {
        if (page->enable())
            page->render();
    }

    if (mouse.enable())
        mouse.render();
    window.update();
}

void engix::Application::handleInput()
{
    handleEvents();
    handleKeyboard();
    handleMouseMovement();
}

void engix::Application::handleKeyboard()
{
    auto currentKeyboardState = SDL_GetKeyboardState(nullptr);

    if (input.isReading)
    {
    }
}

void engix::Application::handleMouseMovement()
{
    auto prevPosition = input.cursor.position;
    int x, y;
    SDL_GetMouseState(&x, &y);
    input.cursor.position = Vector2d(x, y) / roundPixelScale;
    input.cursor.movement = prevPosition - input.cursor.position;
}

void engix::Application::handleMouseDown(SDL_MouseButtonEvent e)
{
    switch (e.button)
    {
    case SDL_BUTTON_LEFT:
        input.cursor.state |= Input::Cursor::LEFT;
        break;
    }
}

void engix::Application::handleMouseUp(SDL_MouseButtonEvent e)
{
    switch (e.button)
    {
    case SDL_BUTTON_LEFT:
        input.cursor.state ^= Input::Cursor::LEFT;
        break;
    }
}

static std::u16string fromU8toU16(const char* str)
{
    constexpr encoding::Encoding format =
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    encoding::UTF16BE;
    #else
    encoding::UTF16LE;
    #endif
    return encoding::convert<char, char16_t>(std::string(str), encoding::Format(encoding::UTF8, format));
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
                input.cursor.isShown = true;
                mouse.enable(true);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                input.cursor.isShown = false;
                mouse.enable(false);
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouseDown(e.button);
            break;
        case SDL_MOUSEBUTTONUP:
            handleMouseUp(e.button);
            break;
        case SDL_KEYDOWN:
            onKeyDown(e);
            break;
        case SDL_TEXTINPUT:
            auto result = fromU8toU16(e.text.text);
            auto shift = result.size();
            input.text.insert(input.textCursor, std::move(result));
            input.textCursor += static_cast<int>(shift);
            break;
        }
    }
}

void engix::Application::onKeyDown(const SDL_Event& event)
{
    if (input.isReading)
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_BACKSPACE:
            if (input.text.size() == 0)
            {
                break;
            }
            input.text.erase(input.textCursor - 1, 1);
            input.textCursor--;
            break;
        case SDL_SCANCODE_LEFT:
            if (input.textCursor == 0)
            {
                break;
            }
            input.textCursor--;
            break;
        case SDL_SCANCODE_RIGHT:
            if (input.textCursor == (input.text.size()))
            {
                break;
            }
            input.textCursor++;
            break;
        }
    }
}