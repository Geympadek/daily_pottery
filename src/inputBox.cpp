#include "inputBox.h"

engix::InputBox::InputBox(shared_ptr<BoxTemplate> background) : TextBox(background)
{
}

engix::InputBox::InputBox(shared_ptr<BoxTemplate> background, int width, int height) : TextBox(background, width, height)
{
}

void engix::InputBox::update(const Mouse &mouse)
{
    TextBox::update(mouse);

    if (_isActive)
    {
        
    }
}
