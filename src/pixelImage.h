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
        size_t width = 0;
        size_t height = 0;

        PixelImage() noexcept {}
        PixelImage(size_t width, size_t height) noexcept;
        PixelImage(Pixels pixels, size_t width, size_t height) noexcept;
        PixelImage(const SmartSDLSurface& surface) noexcept;

        SmartSDLSurface createSDLSurface() const;

        Color& get(size_t x, size_t y) noexcept {return pixels[x + y * width];}
        const Color& get(size_t x, size_t y) const noexcept {return pixels[x + y * width];}

        PixelImage getPart(Rect clip) const noexcept;
        static PixelImage getPart(const PixelImage& origin, Rect clip) noexcept {return origin.getPart(clip);}
        void draw(const PixelImage& src, Vector2s position) noexcept;
        void draw(const PixelImage& src, Rect area) noexcept;
    };
}