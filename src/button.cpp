#include "button.h"

engix::Button::Button(std::shared_ptr<Texture> texture) : VisualElement(std::move(texture))
{
    _width /= 3;
}

engix::Button::Button(std::shared_ptr<Texture> texture, int width, int height) : VisualElement(std::move(texture), width, height)
{
}

void engix::Button::render() const
{
    int shift;
    if (_isClicked)
        shift = _width * 2;
    else if (_isAbove)
        shift = _width;
    else
        shift = 0;
    
    Rect clip(Vector2i{shift, 0}, _width, _height);
    
    _texture->render(_position, clip, _scale, _rotation, _position, _flip, _scaling);
    renderContent();
}
