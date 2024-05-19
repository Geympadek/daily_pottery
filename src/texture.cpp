#include "texture.h"

using namespace engix;

engix::Texture::Texture(Texture &&texture) noexcept
{
    _pixelImage = std::move(texture._pixelImage);
    sdlTexture = std::move(texture.sdlTexture);
    _isLoaded = texture._isLoaded;
    texture._isLoaded = false;
}

void engix::Texture::swap(Texture &texture) noexcept
{
    std::swap(_pixelImage, texture._pixelImage);
    std::swap(sdlTexture, texture.sdlTexture);
    std::swap(_isLoaded, texture._isLoaded);
}

Texture &engix::Texture::operator=(Texture texture) noexcept
{
    swap(texture);
    return *this;
}

engix::Texture::~Texture()
{
    free();
}

static SmartSDLTexture loadSDLTexture(SDL_Surface* surface)
{
    auto texture = makeSmartTexture(SDL_CreateTextureFromSurface(gRenderer, surface));
    if (texture == nullptr)
    {
        std::string msg("Unable to load texture! IMG_Error: ");
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
    return texture;
}

static SmartSDLSurface loadSurface(const assets::Path& path)
{
    auto surface = makeSmartSurface(IMG_Load(path.c_str()));
    if (surface == nullptr)
    {
        std::string msg("Unable to load surface! IMG_Error: ");
        msg += IMG_GetError();
        throw std::runtime_error(msg);
    }
    return surface;
}

Texture engix::Texture::load(const assets::Path &path)
{
    Texture texture;
    
    auto surface = loadSurface(path);
    
    texture._pixelImage = surface;
    texture.sdlTexture = loadSDLTexture(surface.get());
    texture._isLoaded = true;
    return texture;
}

Texture engix::Texture::load(const Texture &sheet, Rect clip)
{
    return load(sheet.pixelImage(clip));
}

Texture engix::Texture::load(PixelImage image)
{
    Texture texture;

    texture._pixelImage = std::move(image);

    auto surface = texture._pixelImage.createSDLSurface();
    texture.sdlTexture = loadSDLTexture(surface.get());
    texture._isLoaded = true;
    return texture;
}

Texture engix::Texture::load(size_t width, size_t height, 
    const PixelImage &left, const PixelImage &up, const PixelImage &right, const PixelImage &down, 
    const PixelImage &luCorner, const PixelImage &ruCorner, const PixelImage &ldCorner, const PixelImage &rdCorner, 
    const PixelImage &fill)
{
    PixelImage pixelImage(width, height);
    auto& pixels = pixelImage.pixels;

    //Inner box
    Vector2i leftUp(luCorner.width, luCorner.height);
    Vector2i rightDown(width - rdCorner.width, height - rdCorner.height);

    pixelImage.draw(luCorner, Vector2i{0, 0});
    pixelImage.draw(ruCorner, Vector2i{rightDown.x, 0});
    pixelImage.draw(ldCorner, Vector2i{0, rightDown.y});
    pixelImage.draw(rdCorner, rightDown);

    //Left
    for (size_t y = leftUp.y; y < rightDown.y; y+=left.height)
    {
        Vector2i pos = {0, y};
        auto diff = rightDown.y - y;
        if (diff < left.height)
        {
            pixelImage.draw(left.getPart(Rect{Vector2i{}, left.width, left.height - diff}), pos);
        }
        else
        {
            pixelImage.draw(left, pos);
        }
    }
    //Right
    for (size_t y = leftUp.y; y < rightDown.y; y+=right.height)
    {
        Vector2i pos = {rightDown.x, y};
        auto diff = rightDown.y - y;
        if (diff < right.height)
        {
            pixelImage.draw(right.getPart(Rect{Vector2i{}, right.width, right.height - diff}), pos);
        }
        else
        {
            pixelImage.draw(right, pos);
        }
    }
    //Up
    for (size_t x = leftUp.x; x < rightDown.x; x+=up.width)
    {
        Vector2i pos = {x, 0};
        auto diff = rightDown.x - x;
        if (diff < up.width)
        {
            pixelImage.draw(up.getPart(Rect{Vector2i{}, up.width, up.height - diff}), pos);
        }
        else
        {
            pixelImage.draw(up, pos);
        }
    }
    //Down
    for (size_t x = leftUp.x; x < rightDown.x; x+=down.width)
    {
        Vector2i pos = {x, rightDown.y};
        auto diff = rightDown.x - x;
        if (diff < down.width)
        {
            pixelImage.draw(down.getPart(Rect{Vector2i{}, down.width, down.height - diff}), pos);
        }
        else
        {
            pixelImage.draw(down, pos);
        }
    }

    auto areaWidth = rightDown.x - leftUp.x;
    auto areaHeight = rightDown.y - leftUp.y;
    //Inside area
    for (size_t y = leftUp.y; y < rightDown.y; y += fill.height)
    {
        for (size_t x = leftUp.x; x < rightDown.x; x += fill.width)
        {
            Rect clip{{0, 0}, std::min(rightDown.x - x, fill.width), std::min(rightDown.y - y, fill.height)};

            pixelImage.draw(fill.getPart(clip), {x, y});
        }
    }
    
    return load(pixelImage);
}

void engix::Texture::free() noexcept
{
    if (!_isLoaded)
        return;
    
    std::cout << "Texture deallocated\n";
    
    _isLoaded = false;
    sdlTexture = nullptr;
    _pixelImage.width = 0;
    _pixelImage.height = 0;
    _pixelImage.pixels = {};
}

void engix::Texture::render(Vector2i position, double scale, Rotation rotation, Vector2i center, Flip flip, Scaling scaling) const
{
    if (!_isLoaded)
        return;

    double appliedScale = scale * ((scaling == Scaling::STRETCH) ? pixelScale : std::floor(pixelScale));

    Vector2i renderPosition = position * appliedScale;
    center *= appliedScale;

    SDL_Rect renderRect = {renderPosition.x, renderPosition.y};

    renderRect.w = static_cast<int>(_pixelImage.width * appliedScale);
    renderRect.h = static_cast<int>(_pixelImage.height * appliedScale);

    SDL_Point sdlCenter{center.x, center.y};

    SDL_RenderCopyEx(gRenderer, sdlTexture.get(), nullptr, &renderRect, rotation.degs(), &sdlCenter, static_cast<SDL_RendererFlip>(flip));
}

void engix::Texture::render(Vector2i position, Rect clip, double scale, Rotation rotation, Vector2i center, Flip flip, Scaling scaling) const
{
    if (!_isLoaded)
        return;

    double appliedScale = (scaling == Scaling::STRETCH) ? pixelScale : roundPixelScale;

    Vector2i renderPosition = position * appliedScale;

    SDL_Rect renderRect = {renderPosition.x, renderPosition.y};

    renderRect.w = static_cast<int>(clip.width * appliedScale);
    renderRect.h = static_cast<int>(clip.height * appliedScale);

    SDL_Point sdlCenter{center.x, center.y};
    
    SDL_Rect sdlClip = clip;
    SDL_RenderCopyEx(gRenderer, sdlTexture.get(), &sdlClip, &renderRect, rotation.degs(), &sdlCenter, static_cast<SDL_RendererFlip>(flip));
}

void engix::Texture::setColor(Color color)
{
    SDL_SetTextureColorMod(sdlTexture.get(), color.red, color.green, color.blue);
    SDL_SetTextureAlphaMod(sdlTexture.get(), color.alpha);
}

