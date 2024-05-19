#include "visualelement.h"

engix::VisualElement::VisualElement(std::shared_ptr<Texture> texture,
    Vector2d relativePos, 
    std::function<void(VisualElement *, const Mouse &)> onClick, 
    std::function<void(VisualElement *, const Mouse &)> onHoverStart, 
    std::function<void(VisualElement *, const Mouse &)> onHoverEnd, 
    Rect srcRect, 
    Align horizontalAlign, 
    Align verticalAlign,
    Texture::Scaling scaling,
    Texture::Flip flip)
: FixedDrawable(texture, {0, 0}, scaling, flip), _width(texture->width()), _height(texture->height()), _onClick(onClick), _onHoverStart(onHoverStart), _onHoverEnd(onHoverEnd), _srcRect(srcRect), _horizontalAlign(horizontalAlign), _verticalAlign(verticalAlign), _relativePos(relativePos)
{
    updatePos();
}

engix::VisualElement::VisualElement(std::shared_ptr<Texture> texture, 
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
: FixedDrawable(std::move(texture), {0, 0}, scaling, flip), _width(width), _height(height), _onClick(onClick), _onHoverStart(onHoverStart), _onHoverEnd(onHoverEnd), _srcRect(srcRect), _horizontalAlign(horizontalAlign), _verticalAlign(verticalAlign), _relativePos(relativePos)
{
    updatePos();
}

void engix::VisualElement::update(const Mouse &mouse)
{
    if (Vector2i::isAbove(_position, _width, _height, mouse.position()))
    {
        if (!_isAbove)
        {
            _onHoverStart(this, mouse);
            _isAbove = true;
        }
        bool leftClick = mouse.state() & Mouse::State::LEFT;
        if (!_isFocused)
        {
            _isFocused = leftClick && Vector2i::isAbove(_position, _width, _height, mouse.position());
            _onClick(this, mouse);
        }
        else if (!leftClick)
        {
            _isFocused = false;
        }
    }
    else
    {
        if (_isAbove)
        {
            _onHoverEnd(this, mouse);
            _isAbove = false;
        }
    }
}

void engix::VisualElement::render() const
{
    Vector2d shift;

    auto textureWidth = _texture->width();
    auto textureHeight = _texture->height();

    auto width = (_horizontalRepeat == Repeat::NONE ? std::min(_width, textureWidth) : _width);
    auto height = (_verticalRepeat == Repeat::NONE ? std::min(_height, textureHeight) : _height);

    for (size_t y = 0; y < height; y += textureHeight)
    {
        for (size_t x = 0; x < width; x += textureWidth)
        {
            Rect clip{{0, 0}, std::min(width - x, textureWidth), std::min(height - y, textureHeight)};

            _texture->render(_position + Vector2d{x, y}, clip, _scale, _rotation, _position, _flip, _scaling);
        }
    }
}

void engix::VisualElement::updatePos()
{
    switch (_horizontalAlign)
    {
    case Align::START:
        _position.x = _relativePos.x;
        break;
    case Align::CENTER:
        _position.x = _relativePos.x + (_srcRect.width - _width) * 0.5;
        break;
    case Align::END:
        _position.x = _srcRect.width - _width - _relativePos.x;
        break;
    }
    switch (_verticalAlign)
    {
    case Align::START:
        _position.y = _relativePos.y;
        break;
    case Align::CENTER:
        _position.y = _relativePos.y + (_srcRect.height - _height) * 0.5;
        break;
    case Align::END:
        _position.y = _srcRect.height - _height - _relativePos.y;
        break;
    }
    _position += _srcRect.start;
}
