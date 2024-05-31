#pragma once

#include "texture.h"

namespace engix
{
    class BoxTemplate : public Resource
    {
    public:
        BoxTemplate() {}
        BoxTemplate(const assets::Path& path) : BoxTemplate(load(path)) {}
        
        static BoxTemplate load(const assets::Path& path);
        Texture genTexture(int width, int height) const;

        PixelImage left, up, right, down;
        PixelImage leftUp, rightUp, leftDown, rightDown;
        PixelImage fill;
    };
}