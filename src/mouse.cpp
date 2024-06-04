#include "mouse.h"

engix::Mouse::Mouse(std::shared_ptr<Texture> texture) : FixedDrawable(std::move(texture))
{
    _scaling = Texture::Scaling::NONE;
}

void engix::Mouse::update(Input &input)
{
    _position = input.cursor.position;
}

void engix::Mouse::render() const
{
    FixedDrawable::render();
}
