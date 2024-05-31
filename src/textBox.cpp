#include "textBox.h"

engix::TextBox::TextBox(std::shared_ptr<BoxTemplate> background) : _background(std::move(background))
{
}

engix::TextBox::TextBox(std::shared_ptr<BoxTemplate> background, int width, int height)
: _background(std::move(background)), _horizontalSize(Size::FIXED), _verticalSize(Size::FIXED)
{
}

void engix::TextBox::update(const Mouse &mouse)
{
    if (_updateSize)
    {
        auto size = _font->findSize(_text);
        if (_horizontalSize == Size::AUTO)
        {
            _width = _paddingLeft + _paddingRight + size.x;
        }
        if (_verticalSize == Size::AUTO)
        {
            _height = _paddingUp + _paddingDown + size.y;
        }
        _updateSize = false;
        _updateBackground = true;
    }
    if (_updateBackground)
    {
        _texture = std::make_shared<Texture>(_background->genTexture(_width, _height));
        _updateBackground = false;
    }
    VisualElement::update(mouse);
}

void engix::TextBox::render() const
{
    VisualElement::render();

    if (_font != nullptr)
    {
        _font->render(_text, _position + Vector2i(_paddingLeft, _paddingUp), _textColor);
    }
}
