#include "inputBox.h"

engix::InputBox::InputBox(shared_ptr<BoxTemplate> background, shared_ptr<Texture> textCursor) : TextBox(background), _textCursor(std::move(textCursor))
{
    _cursorClock.start();
}

engix::InputBox::InputBox(shared_ptr<BoxTemplate> background, int width, int height, shared_ptr<Texture> textCursor) : TextBox(background, width, height), _textCursor(std::move(textCursor))
{
    _cursorClock.start();
}

void engix::InputBox::render() const
{
    TextBox::render();
    if (_showCursor)
        _textCursor->render(_position + _cursorPosition + Vector2i(_paddingLeft, _paddingUp), _scale, _rotation, _position, _flip, _scaling);
}

void engix::InputBox::update(Input &input)
{
    bool prevActive = _isActive;
    TextBox::update(input);
    if (!prevActive && _isActive)
        input.isReading = true;
    else if (prevActive && !_isActive)
        input.isReading = false;

    if (_isActive)
    {
        input.isReading = true;
        if (_text != input.text)
        {
            text(input.text);
        }
        _cursorPosition = Vector2i(_font->findSize(input.text.substr(0, input.textCursor)).x, 0) + _font->cursorOffset();
    }
    _showCursor = _isActive && (static_cast<int>(_cursorClock.millis()) % 1000 < 500);
}
