#include "mouse.h"

engix::Mouse::Mouse(std::shared_ptr<Texture> texture) : FixedDrawable(std::move(texture))
{
    _scaling = Texture::Scaling::NONE;
}

void engix::Mouse::update()
{
    auto prevPos = _position;
    int x, y;
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
