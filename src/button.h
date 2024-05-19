#pragma once

#include "contentWrap.h"
#include <functional>

namespace engix
{
    class Button : public VisualElement
    {
    public:
        Button() {}
        Button(std::shared_ptr<Texture> texture, 
            Vector2d relativePos, 
            std::function<void(VisualElement*, const Mouse&)> onClick = [](VisualElement*, const Mouse&){},
            std::function<void(VisualElement*, const Mouse&)> onHoverStart = [](VisualElement*, const Mouse&){},
            std::function<void(VisualElement*, const Mouse&)> onHoverEnd = [](VisualElement*, const Mouse&){},
            Rect srcRect = gScreen,
            Align horizontalAlign = Align::START,
            Align verticalAlign = Align::START,
            Texture::Scaling scaling = Texture::Scaling::NONE,
            Texture::Flip flip = Texture::Flip::NONE);

        Button(std::shared_ptr<Texture> texture, 
            Vector2d relativePos, 
            size_t width,
            size_t height,
            std::function<void(VisualElement*, const Mouse&)> onClick = [](VisualElement*, const Mouse&){},
            std::function<void(VisualElement*, const Mouse&)> onHoverStart = [](VisualElement*, const Mouse&){},
            std::function<void(VisualElement*, const Mouse&)> onHoverEnd = [](VisualElement*, const Mouse&){},
            Rect srcRect = gScreen,
            Align horizontalAlign = Align::START,
            Align verticalAlign = Align::START,
            Texture::Scaling scaling = Texture::Scaling::NONE,
            Texture::Flip flip = Texture::Flip::NONE);
    
        virtual void render() const override;
    protected:
    };
}