#pragma once

#include "textBox.h"

namespace engix
{
    class InputBox : public TextBox
    {
    public:
        InputBox() {}
        InputBox(shared_ptr<BoxTemplate> background);
        InputBox(shared_ptr<BoxTemplate> background, int width, int height);

        virtual void update(const Mouse& mouse) override;
    protected:
    };
}