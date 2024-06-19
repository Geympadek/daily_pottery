#include "textBox.h"

engix::TextBox::TextBox(std::shared_ptr<BoxTemplate> background) : _background(std::move(background))
{
    _enable = true;
}

engix::TextBox::TextBox(std::shared_ptr<BoxTemplate> background, int width, int height)
: _background(std::move(background)), _horizontalSize(Size::FIXED), _verticalSize(Size::FIXED)
{
    _width = width;
    _height = height;
}

void engix::TextBox::update(Input& input)
{
    if (_updateSize)
    {
        updateSize();
        _updateSize = false;
    }
    if (_updateBackground)
    {
        _texture = std::make_shared<Texture>(_background->genTexture(_width, _height));
        _updateBackground = false;
        _updatePos = true;
    }
    VisualElement::update(input);
}

void engix::TextBox::render() const
{
    VisualElement::render();

    if (_font != nullptr)
    {
        _font->render(_text, _position + _textPosition + Vector2i(_paddingLeft, _paddingUp), _textColor);
    }
}

void engix::TextBox::updateSize()
{
    auto size = _font->findSize(_text);
    if (_horizontalSize == Size::AUTO)
    {
        auto width = _paddingLeft + _paddingRight + size.x;
        if (width < _minWidth)
        {
            width = _minWidth;
        }
        else if (width > _maxWidth)
        {
            width = _maxWidth;
        }
        if (_width != width)
        {
            _updateBackground = true;
            _width = width;
        }
    }
    if (_verticalSize == Size::AUTO)
    {
        auto height = _paddingUp + _paddingDown + size.y;
        if (height < _minHeight)
        {
            height = _minHeight;
        }
        else if (height > _maxHeight)
        {
            height = _maxHeight;
        }
        if (_height != height)
        {
            _updateBackground = true;
            _height = height;
        }
    }
    
    auto pureWidth = _width - _paddingLeft - _paddingRight;
    auto pureHeight = _height - _paddingUp - _paddingDown;
    switch (_horizontalTextAlign)
    {
    case Align::START:
        _textPosition.x = 0;
        break;
    case Align::CENTER:
        _textPosition.x = (pureWidth - size.x) / 2;
        break;
    case Align::END:
        _textPosition.x = pureWidth - size.x;
        break;
    }
    switch (_verticalTextAlign)
    {
    case Align::START:
        _textPosition.y = 0;
        break;
    case Align::CENTER:
        _textPosition.y = (pureHeight - size.y) / 2;
        break;
    case Align::END:
        _textPosition.y = pureHeight - size.y;
        break;
    }
}