#pragma once

#include "fixedDrawable.h"
#include <iostream>
#include <SDL.h>

namespace engix
{
    //Cursor incapsulation
    class Mouse : public FixedDrawable 
    {
    public:
        Mouse() {}
        Mouse(std::shared_ptr<Texture> texture);
        virtual void update(Input& input) override;
        virtual void render() const override;
    };
}