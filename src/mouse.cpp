#include "mouse.h"

engix::Mouse::Mouse(std::shared_ptr<Texture> texture) : FixedDrawable(texture, {}, Texture::Scaling::NONE)
{
}

void engix::Mouse::update()
{
    auto prevPos = _position;
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    _position = Vector2d{x, y} / roundPixelScale;
    _movement = _position - prevPos;
}

void engix::Mouse::render() const
{
    if (_isShown)
    {
        FixedDrawable::render();
    }
}
