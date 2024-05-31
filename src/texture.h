#pragma once

#include <SDL_image.h>
#include <vector>
#include <memory>
#include <algorithm>

#include "vector2.h"
#include "color.h"
#include "window.h"

#include "resource.h"
#include "rotation.h"

#include "pixelImage.h"

#include "rect.h"

#include "path.h"

namespace engix
{
    extern SDL_Renderer* gRenderer;

    class Texture : public Resource
    {
    public:
        enum class Flip {NONE = SDL_FLIP_NONE, HORIZONTAL = SDL_FLIP_HORIZONTAL, VERTICAL = SDL_FLIP_VERTICAL};
        enum class Scaling {NONE, STRETCH};

        Texture() noexcept {}
        template <class... Args>
        Texture(Args&&... params) : Texture(load(std::forward<Args>(params)...)) {}

        Texture(const Texture& texture) : Texture(load(texture._pixelImage)) {}
        Texture(Texture& texture) : Texture(static_cast<const Texture&>(texture)) {}
        Texture(Texture&& texture) noexcept;

        void swap(Texture& texture) noexcept;

        Texture& operator=(Texture texture) noexcept;

        ~Texture();

        static Texture load(const assets::Path& path);
        static Texture load(const Texture& sheet, Rect clip);
        static Texture load(PixelImage image);

        static Texture load(int width, int height, 
            const Texture& left, const Texture& up, const Texture& right, const Texture& down, 
            const Texture& luCorner, const Texture& ruCorner, const Texture& ldCorner, const Texture& rdCorner, 
            const Texture& fill)
        {
            return load(width, height,
                left._pixelImage, up._pixelImage, right._pixelImage, down._pixelImage,
                luCorner._pixelImage, ruCorner._pixelImage, ldCorner._pixelImage, rdCorner._pixelImage,
                fill._pixelImage);
        }
        static Texture load(int width, int height, const Texture& sheet,
            Rect left, Rect up, Rect right, Rect down, 
            Rect luCorner, Rect ruCorner, Rect ldCorner, Rect rdCorner,
            Rect fill)
        {
            return load(width, height,
                sheet.pixelImage(left), sheet.pixelImage(up), sheet.pixelImage(right), sheet.pixelImage(down),
                sheet.pixelImage(luCorner), sheet.pixelImage(ruCorner), sheet.pixelImage(ldCorner), sheet.pixelImage(rdCorner),
                sheet.pixelImage(fill));
        }
        static Texture load(int width, int height,
            const PixelImage& left, const PixelImage& up, const PixelImage& right, const PixelImage& down,
            const PixelImage& luCorner, const PixelImage& ruCorner, const PixelImage& ldCorner, const PixelImage& rdCorner,
            const PixelImage& fill);

        virtual void free() noexcept;

        void render(Vector2i position, double scale = 1.0, Rotation rotation = {}, Vector2i center = {}, Flip flip = Flip::NONE, Scaling scaling = Scaling::NONE) const;
        void render(Vector2i position, Rect clip, double scale = 1.0, Rotation rotation = {}, Vector2i center = {}, Flip flip = Flip::NONE, Scaling scaling = Scaling::NONE) const;
    
        void setColor(Color color);
    public:
        const Pixels& pixels() const noexcept {return _pixelImage.pixels;}
        Pixels pixels(Rect clip) const noexcept {return _pixelImage.getPart(clip).pixels;}
        const PixelImage& pixelImage() const noexcept {return _pixelImage;}
        PixelImage pixelImage(Rect clip) const noexcept {return _pixelImage.getPart(clip);}
        
        int width() const noexcept {return _pixelImage.width; }
        void width(int width) noexcept {_pixelImage.width = width;}
        int height() const noexcept {return _pixelImage.height;}
        void height(int height) noexcept {_pixelImage.height = height;}
    private:
        PixelImage _pixelImage;
        SmartSDLTexture sdlTexture = makeSmartTexture(nullptr);
    };
}
