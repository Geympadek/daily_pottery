#include "pixelImage.h"

using namespace engix;

PixelImage::PixelImage(int width, int height) noexcept : width(width), height(height)
{
    resize(width, height);
}

PixelImage::PixelImage(Pixels pixels, int width, int height) noexcept : pixels(std::move(pixels)), width(width), height(height)
{
}

engix::PixelImage::PixelImage(const SmartSDLSurface& surface) noexcept
{
    resize(surface->w, surface->h);
    auto numberOfPixels = width * height;

    auto data = static_cast<uint32_t*>(surface->pixels);
    for (int i = 0; i < numberOfPixels; i++)
    {
        uint8_t red, green, blue, alpha;
        SDL_GetRGBA(data[i], surface->format, &red, &green, &blue, &alpha);
        pixels[i] = Color(red, green, blue, alpha);
    }
}

void engix::PixelImage::resize(int width, int height) noexcept
{
    this->width = width; 
    this->height = height;
    pixels.resize(width * height, Color::TRANSPARENT);
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
    auto numberOfPixels = pixels.size();
    for (int i = 0; i < numberOfPixels; ++i) 
    {
        dataPixels[i] = SDL_MapRGBA(surface->format, pixels[i].red, pixels[i].green, pixels[i].blue, pixels[i].alpha);
    }

    if (SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface.get());
    }
    return surface;
}

PixelImage PixelImage::getPart(Rect clip) const noexcept
{
    PixelImage result;
    getPart(result, clip);
    return result;
}

PixelImage &engix::PixelImage::getPart(PixelImage &dest, Rect clip) const noexcept
{
    dest.resize(clip.width, clip.height);

    for (int i = 0, y = 0; y < clip.height; y++)
    {
        for (int x = 0; x < clip.width; x++, i++)
        {
            dest.pixels[i] = get(clip.start.x + x, clip.start.y + y);
        }
    }
    return dest;
}

void PixelImage::draw(const PixelImage &src, Vector2i position) noexcept
{
    const auto numberOfPixels = pixels.size();
    for (int y = 0; y < src.height; y++)
    {
        for (int x = 0; x < src.width; x++)
        {
            auto calcX = position.x + x;
            auto calcY = position.y + y;

            if (calcX < width && calcY < height)
                this->get(calcX, calcY) = src.get(x, y);
            else
                continue;
        }
    }
}

void engix::PixelImage::draw(const PixelImage &src, Rect area) noexcept
{
    auto end = Vector2i{area.start.x + area.width, area.start.y + area.height};
    
    PixelImage buffer;
    for (int y = area.start.y; y < end.y; y += src.height)
    {
        for (int x = area.start.x; x < end.x; x += src.width)
        {
            Vector2s pos(x, y);
            auto delta = end - pos;
            if (delta.x < src.width || delta.y < src.height)
            {
                Rect clip(delta.x, delta.y);
                draw(src.getPart(buffer, clip), pos);
            }
            else
            {
                draw(src, pos);
            }
        }
    }
}

void engix::PixelImage::fillWith(Color color, Rect area) noexcept
{
    auto end = area.start + Vector2i(area.width, area.height);
    for (int y = area.start.y; y < end.y; y++)
    {
        for (int x = area.start.x; x < end.x; x++)
        {
            get(x, y) = color;
        }
    }
}