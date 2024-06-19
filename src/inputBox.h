#pragma once

#include "textBox.h"
#include "clock.h"

namespace engix
{
    class InputBox : public TextBox
    {
    public:
        InputBox() {}
        InputBox(shared_ptr<BoxTemplate> background, shared_ptr<Texture> textCursor);
        InputBox(shared_ptr<BoxTemplate> background, int width, int height, shared_ptr<Texture> textCursor);

        virtual void render() const override;
        virtual void update(Input& input) override;
    protected:
        shared_ptr<Texture> _textCursor;
        bool _showCursor;

        Vector2i _cursorPosition;
    };
}