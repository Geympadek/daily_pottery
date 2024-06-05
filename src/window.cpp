#include "window.h"

SDL_Renderer* engix::gRenderer;
void engix::Window::init(const std::string& title, int width, int height)
{
    this->width = width;
    this->height = height;
    this->title = std::move(title);

    updatePixelScale();
    updateScreen();

    int windowFlags = SDL_WINDOW_SHOWN;
    window = nullptr;
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
    if (window == nullptr)
    {
        std::string msg = "Unable to open window! SDL_Error: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
    
    int renderFlags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
    gRenderer = nullptr;

    gRenderer = SDL_CreateRenderer(window, -1, renderFlags);
    if (gRenderer == nullptr)
    {
        std::string msg = "Unable to create renderer! SDL_Error: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

engix::Window::~Window()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
}

void engix::Window::setTitle(const char *title) noexcept
{
    this->title = title;
    SDL_SetWindowTitle(window, title);
}

void engix::Window::setResolution(int width, int height) noexcept
{
    this->width = width;
    this->height = height;
    updatePixelScale();
    updateScreen();
    SDL_SetWindowSize(window, width, height);
}

double engix::pixelScale;
double engix::roundPixelScale;

void engix::Window::updatePixelScale() const noexcept
{
    pixelScale = ((double)width / MIN_PIXEL_WIDTH + (double)height / MIN_PIXEL_HEIGHT) * 0.5;
    roundPixelScale = std::floor(pixelScale);
}

void engix::Window::update() noexcept
{
    SDL_RenderPresent(gRenderer);
}

engix::Rect engix::gScreen;
void engix::Window::updateScreen() noexcept
{
    //MIN_PIXEL_WIDTH / roundPixelScale = x / pixelScale;
    //x = MIN_PIXEL_WIDTH * pixelScale / roundPixelScale;
    gScreen.width = static_cast<int>(std::round(MIN_PIXEL_WIDTH * pixelScale / roundPixelScale));
    gScreen.height = static_cast<int>(std::round(MIN_PIXEL_HEIGHT * pixelScale / roundPixelScale));
}

void engix::Window::fill(Color color) noexcept
{
    SDL_SetRenderDrawColor(gRenderer, color.red, color.green, color.blue, color.alpha);
    SDL_RenderClear(gRenderer);
}