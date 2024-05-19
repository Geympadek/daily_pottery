#include "pixelImage.h"

using namespace engix;

PixelImage::PixelImage(size_t width, size_t height) noexcept : width(width), height(height)
{
    pixels.resize(width * height, Color::TRANSPARENT);
}

PixelImage::PixelImage(Pixels pixels, size_t width, size_t height) noexcept : pixels(std::move(pixels)), width(width), height(height)
{
}

engix::PixelImage::PixelImage(const SmartSDLSurface& surface) noexcept : width(surface->w), height(surface->h)
{
    auto numberOfPixels = width * height;
    pixels.resize(numberOfPixels);

    auto data = static_cast<uint32_t*>(surface->pixels);
    for (size_t i = 0; i < numberOfPixels; i++)
    {
        uint8_t red, green, blue, alpha;
        SDL_GetRGBA(data[i], surface->format, &red, &green, &blue, &alpha);
        pixels[i] = Color(red, green, blue, alpha);
    }
}

SmartSDLSurface engix::PixelImage::createSDLSurface() const
{
    auto surface = makeSmartSurface(SDL_CreateRGBSurfaceWithFormat(0, static_cast<int>(width), static_cast<int>(height), 32, SDL_PIXELFORMAT_RGBA32));
    if (surface == nullptr)
    {
        std::string msg("Unable to create surface! SDL_Error: ");
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }

    if (SDL_MUSTLOCK(surface)) 
    {
        SDL_LockSurface(surface.get());
    }
    
    auto dataPixels = static_cast<uint32_t*>(surface->pixels);

    // auto numberOfPixels = pixels.size();
    // for (size_t i = 0; i < numberOfPixels; ++i) 
    // {
    //     dataPixels[i] = SDL_MapRGBA(surface->format, pixels[i].red, pixels[i].green, pixels[i].blue, pixels[i].alpha);
    // }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto index = y * width + x;

            Uint8 r = (Uint8)pixels[index].red;
            Uint8 g = (Uint8)pixels[index].green;
            Uint8 b = (Uint8)pixels[index].blue;
            Uint8 a = pixels[index].alpha;  // Fully opaque

            Uint32 pixel = SDL_MapRGBA(surface->format, r, g, b, a);

            dataPixels[index] = pixel;
        }
    }

    if (SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface.get());
    }
    return surface;
}

PixelImage PixelImage::getPart(Rect clip) const noexcept
{
    PixelImage result(clip.width, clip.height);
    
    for (size_t i = 0, y = 0; y < clip.height; y++)
    {
        for (size_t x = 0; x < clip.width; x++, i++)
        {
            result.pixels[i] = get(clip.start.x + x, clip.start.y + y);
        }
    }
    return result;
}

void PixelImage::draw(const PixelImage &src, Vector2s position) noexcept
{
    const auto numberOfPixels = pixels.size();
    for (size_t y = 0; y < src.height; y++)
    {
        for (size_t x = 0; x < src.width; x++)
        {
            auto calcX = position.x + x;
            auto calcY = position.y + y;

            if (calcX < width && calcY < height)
            {
                this->get(calcX, calcY) = src.get(x, y);
            }
        }
    }
}
