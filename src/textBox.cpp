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
    if (input.selection != _selection)
    {
        _selection = input.selection;

        PixelImage selectedArea(_width, _height);
        
        _font->map(_text, [this, &selectedArea](Vector2i cursor, char16_t c, size_t i, const Font::CharInfo& cInfo){
            if (!_selection.contains(i))
            {
                return;
            }

            Rect clip(Vector2i(_paddingLeft, _paddingUp) + cursor + _font->selectionOffset(), cInfo.width + _font->selectedLeft() + _font->selectedRight(), _font->selectedHeight());
            selectedArea.fillWith(_selectionColor, clip);
        });
        _selectedArea = Texture::load(selectedArea);
    }

    VisualElement::update(input);
}

void engix::TextBox::render() const
{
    VisualElement::render();

    _selectedArea.render(_position, _scale, _rotation, _position, _flip, _scaling);

    if (_font != nullptr)
    {
        _font->render(_text, _position + Vector2i(_paddingLeft, _paddingUp), _textColor, _selection, _selectedTextColor);
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
}