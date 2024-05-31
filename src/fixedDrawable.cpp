#include "fixedDrawable.h"

engix::FixedDrawable::FixedDrawable(std::shared_ptr<Texture> texture) : _texture(std::move(texture))
{

}

void engix::FixedDrawable::render() const
{
    assert(_texture);
    _texture->render(_position, _scale, _rotation, _position, _flip, _scaling);
}
