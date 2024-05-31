#pragma once

#include "visualelement.h"

namespace engix
{
    class Button : public VisualElement
    {
    public:
        Button() {}
        Button(std::shared_ptr<Texture> texture);
        Button(std::shared_ptr<Texture> texture, int width, int height);
    
        virtual void render() const override;
    protected:
    };
}