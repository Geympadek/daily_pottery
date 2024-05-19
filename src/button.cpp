#include "button.h"

engix::Button::Button(std::shared_ptr<Texture> texture, 
    Vector2d relativePos, 
    std::function<void(VisualElement *, const Mouse &)> onClick, 
    std::function<void(VisualElement *, const Mouse &)> onHoverStart,
    std::function<void(VisualElement *, const Mouse &)> onHoverEnd, 
    Rect srcRect,
    Align horizontalAlign,
    Align verticalAlign,
    Texture::Scaling scaling,
    Texture::Flip flip)
: VisualElement(texture, relativePos, texture->width() / 3, texture->height(), onClick, onHoverStart, onHoverEnd, srcRect, horizontalAlign, verticalAlign, scaling, flip)
{
}

engix::Button::Button(std::shared_ptr<Texture> texture,
    Vector2d relativePos,
    size_t width,
    size_t height, 
    std::function<void(VisualElement *, const Mouse &)> onClick, 
    std::function<void(VisualElement *, const Mouse &)> onHoverStart, 
    std::function<void(VisualElement *, const Mouse &)> onHoverEnd, 
    Rect srcRect, 
    Align horizontalAlign, 
    Align verticalAlign,
    Texture::Scaling scaling,
    Texture::Flip flip)
: VisualElement(std::move(texture), relativePos, width, height, onClick, onHoverStart, onHoverEnd, srcRect, horizontalAlign, verticalAlign, scaling, flip)
{
}

void engix::Button::render() const
{
    size_t shift;
    if (_isFocused)
        shift = _width * 2;
    else if (_isAbove)
        shift = _width;
    else
        shift = 0;
    
    Rect clip{Vector2i{shift, 0}, _width, _height};

    _texture->render(_position, clip, _scale, _rotation, _position, _flip, _scaling);
}
