#include "textBox.h"

engix::TextBox::TextBox(std::shared_ptr<BoxTemplate> background) : _background(std::move(background))
{
    _enable = true;
}

engix::TextBox::TextBox(std::shared_ptr<BoxTemplate> background, int width, int height)
: _background(std::move(background)), _horizontalSize(Size::FIXED), _verticalSize(Size::FIXED)
{
    _enable = true;
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
        _font->render(_text, _position + Vector2i(_paddingLeft, _paddingUp), _textColor);
    }
}

void engix::TextBox::updateSize()
{
    auto size = _font->findSize(_text);
    if (_horizontalSize == Size::AUTO)
    {
        _width = _paddingLeft + _paddingRight + size.x;
        if (_width < _minWidth)
        {
            _width = _minWidth;
            _updateBackground = true;
        }
        else if (_width > _maxWidth)
        {
            _width = _maxWidth;
            _updateBackground = true;
        }
    }
    if (_verticalSize == Size::AUTO)
    {
        _height = _paddingUp + _paddingDown + size.y;
        if (_height < _minHeight)
        {
            _height = _minHeight;
            _updateBackground = true;
        }
        else if (_height > _maxHeight)
        {
            _height = _maxHeight;
            _updateBackground = true;
        }
    }
}