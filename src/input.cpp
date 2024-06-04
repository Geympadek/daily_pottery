#include "input.h"
#include "window.h"
#include "encoding.h"

using namespace engix;

void Input::update()
{
    if (!lastReading && isReading)
    {
        SDL_StartTextInput();
    }
    else if (lastReading && !isReading)
    {
        SDL_StopTextInput();
    }
    lastReading = isReading;

    handleEvents();
    handleKeyboard();
    handleMouseMovement();
}

void engix::Input::handleEvents()
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
            handleWindowEvent(e.window);
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouseDown(e.button);
            break;
        case SDL_MOUSEBUTTONUP:
            handleMouseUp(e.button);
            break;
        case SDL_KEYDOWN:
            onKeyDown(e.key);
            break;
        case SDL_TEXTINPUT:
            handleTextInput(e.text);
            break;
        }
    }
}

void engix::Input::handleKeyboard()
{
}

void engix::Input::handleMouseMovement()
{
    auto prevPosition = cursor.position;
    int x, y;
    SDL_GetMouseState(&x, &y);
    cursor.position = Vector2d(x, y) / roundPixelScale;
    cursor.movement = prevPosition - cursor.position;
}

void engix::Input::handleWindowEvent(const SDL_WindowEvent &e)
{
    switch (e.event)
    {
    case SDL_WINDOWEVENT_ENTER:
        cursor.isShown = true;
        break;
    case SDL_WINDOWEVENT_LEAVE:
        cursor.isShown = false;
        break;
    }
}

void engix::Input::handleMouseDown(const SDL_MouseButtonEvent& e)
{
    switch (e.button)
    {
    case SDL_BUTTON_LEFT:
        cursor.state |= Cursor::LEFT;
        break;
    }
}

void engix::Input::handleMouseUp(const SDL_MouseButtonEvent& e)
{
    switch (e.button)
    {
    case SDL_BUTTON_LEFT:
        cursor.state ^= Input::Cursor::LEFT;
        break;
    }
}

void engix::Input::handleTextInput(const SDL_TextInputEvent &e)
{
    constexpr encoding::Format format(encoding::UTF8,
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    encoding::UTF16BE
    #else
    encoding::UTF16LE
    #endif
    );

    auto insertedText = encoding::convert<char, char16_t>(std::string(e.text), format);
    text.insert(textCursor, insertedText);
    textCursor += static_cast<int>(insertedText.size());
}

void engix::Input::onKeyDown(const SDL_KeyboardEvent &e)
{
    if (isReading)
    {
        switch (e.keysym.scancode)
        {
        case SDL_SCANCODE_BACKSPACE:
            if (text.size() == 0)
                break;
            
            text.erase(textCursor - 1, 1);
            textCursor--;
            break;
        case SDL_SCANCODE_LEFT:
            if (textCursor == 0)
                break;
            
            textCursor--;
            break;
        case SDL_SCANCODE_RIGHT:
            if (textCursor == text.size())
                break;
            
            textCursor++;
            break;
        }
    }
}
