#pragma once

#include <vector>
#include "color.h"
#include "rect.h"

namespace engix
{
    using Pixels = std::vector<Color>;

    inline void SDLSurfaceDeallocator(SDL_Surface* surface)
    {
        SDL_FreeSurface(surface);
    }
    inline void SDLTextureDeallocator(SDL_Texture* texture)
    {
        SDL_DestroyTexture(texture);
    }

    using SmartSDLSurface = std::unique_ptr<SDL_Surface, decltype(&SDLSurfaceDeallocator)>;
    using SmartSDLTexture = std::unique_ptr<SDL_Texture, decltype(&SDLTextureDeallocator)>;

    inline SmartSDLSurface makeSmartSurface(SDL_Surface* surface)
    {
        return SmartSDLSurface(surface, SDLSurfaceDeallocator);
    }
    inline SmartSDLTexture makeSmartTexture(SDL_Texture* texture)
    {
        return SmartSDLTexture(texture, SDLTextureDeallocator);
    }

    struct PixelImage
    {
        Pixels pixels;
        int width = 0;
        int height = 0;

        PixelImage() noexcept {}
        PixelImage(int width, int height) noexcept;
        PixelImage(Pixels pixels, int width, int height) noexcept;
        PixelImage(const SmartSDLSurface& surface) noexcept;

        void resize(int width, int height) noexcept;

        SmartSDLSurface createSDLSurface() const;

        Color& get(int x, int y) noexcept {return pixels[x + y * width];}
        const Color& get(int x, int y) const noexcept {return pixels[x + y * width];}

        PixelImage getPart(Rect clip) const noexcept;
        PixelImage& getPart(PixelImage& dest, Rect clip) const noexcept;
        void draw(const PixelImage& src, Vector2i position) noexcept;
        void draw(const PixelImage& src, Rect area) noexcept;

        void fillWith(Color color) noexcept {fillWith(color, Rect(0, 0, width, height));}
        void fillWith(Color color, Rect area) noexcept;
    };
}