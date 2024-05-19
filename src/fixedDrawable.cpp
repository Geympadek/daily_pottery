#include "fixedDrawable.h"

engix::FixedDrawable::FixedDrawable(std::shared_ptr<Texture> texture, Vector2d position, Texture::Scaling scaling, Texture::Flip flip)
 : _texture(std::move(texture)), _position(position), _scaling(scaling), _flip(flip)
{
}

void engix::FixedDrawable::render() const
{
    assert(_texture);
    _texture->render(_position, _scale, _rotation, _position, _flip, _scaling);
}
