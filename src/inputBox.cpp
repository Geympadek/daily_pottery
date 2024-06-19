#include "inputBox.h"

engix::InputBox::InputBox(shared_ptr<BoxTemplate> background, shared_ptr<Texture> textCursor) : TextBox(background), _textCursor(std::move(textCursor))
{
}

engix::InputBox::InputBox(shared_ptr<BoxTemplate> background, int width, int height, shared_ptr<Texture> textCursor) : TextBox(background, width, height), _textCursor(std::move(textCursor))
{
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
            text(input.text);

        if (input.cursor.state & Input::Cursor::LEFT)
        {
            auto relCursorPos = input.cursor.position - _position - Vector2i(_paddingLeft, _paddingUp);
            input.textCursor = static_cast<int>(_font->findClosest(_text, relCursorPos));
        }
        _cursorPosition = Vector2i(_font->findSize({_text.begin(), _text.begin() + input.textCursor}).x, 0) + _font->cursorOffset();
    }
    _showCursor = _isActive && input.blinkTextCursor;
}
