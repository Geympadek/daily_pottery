#include "visualelement.h"

engix::VisualElement::VisualElement(std::shared_ptr<Texture> texture) : FixedDrawable(std::move(texture)), _width(_texture->width()), _height(_texture->height())
{
}

engix::VisualElement::VisualElement(std::shared_ptr<Texture> texture, int width, int height) : FixedDrawable(std::move(texture)), _width(width), _height(height)
{
}

void engix::VisualElement::update(Input& input)
{
    FixedDrawable::update(input);

    if (_updatePos)
    {
        updatePos();
        _updatePos = false;
    }

    bool leftClick = input.cursor.state & Input::Cursor::State::LEFT;
    if (rect().isAbove(input.cursor.position))
    {
        if (!_isAbove)
        {
            if (_onHoverStart != nullptr)
            {
                _onHoverStart(this, input);
            }
            _isAbove = true;
        }
        if (!_isClicked && leftClick)
        {
            _isClicked = true;
            _isActive = true;
            if (_onClick != nullptr)
            {
                _onClick(this, input);
            }
        }
    } else
    {
        if (leftClick)
        {
            _isActive = false;
        }
        if (_isAbove)
        {
            if (_onHoverEnd != nullptr)
            {
                _onHoverEnd(this, input);
            }
            _isAbove = false;
        }
    }
    if (!leftClick)
    {
        _isClicked = false;
    }
}

void engix::VisualElement::render() const
{
    FixedDrawable::render();
    renderContent();
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

    auto contentWrap = rect();
    contentWrap.start += Vector2i(_paddingLeft, _paddingUp);
    contentWrap.width -= _paddingRight + _paddingLeft;
    contentWrap.height -= _paddingUp + _paddingDown;

    for (auto it : _content)
    {
        it->srcRect(contentWrap);
    }
}

void engix::VisualElement::renderContent() const
{
    for (auto it : _content)
    {
        if (it->enable())
            it->render();
    }
}
