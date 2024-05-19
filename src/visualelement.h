#pragma once

#include "mouse.h"
#include <functional>

namespace engix
{
    enum class Align {START, CENTER, END};
    enum class Repeat {NONE = 0b0, REPEAT};
    //Any interface element
    class VisualElement : public FixedDrawable
    {
    public:
        VisualElement() {}
        VisualElement(std::shared_ptr<Texture> texture,
            Vector2d relativePos,
            std::function<void(VisualElement*, const Mouse&)> onClick = [](VisualElement*, const Mouse&){},
            std::function<void(VisualElement*, const Mouse&)> onHoverStart = [](VisualElement*, const Mouse&){}, 
            std::function<void(VisualElement*, const Mouse&)> onHoverEnd = [](VisualElement*, const Mouse&){}, 
            Rect srcRect = gScreen, 
            Align horizontalAlign = Align::START, 
            Align verticalAlign = Align::START,
            Texture::Scaling scaling = Texture::Scaling::NONE,
            Texture::Flip flip = Texture::Flip::NONE);

        VisualElement(std::shared_ptr<Texture> texture,
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
        
        virtual void update(const Mouse& mouse);
        virtual void render() const override;
    protected:
        void updatePos();
    public:
        virtual Rect srcRect() const {return _srcRect;}
        virtual void srcRect(Rect rect) {_srcRect = rect; updatePos();}

        virtual void onClick(std::function<void(VisualElement*, const Mouse&)> onClick) {_onClick = onClick;}
        virtual void onHoverStart(std::function<void(VisualElement*, const Mouse&)> onHoverStart) {_onHoverStart = onHoverStart;}
        virtual void onHoverEnd(std::function<void(VisualElement*, const Mouse&)> onHoverEnd) {_onHoverEnd = onHoverEnd;}

        virtual size_t width() const {return _width;}
        virtual void width(size_t width) {_width = width;}
        virtual size_t height() const {return _height;}
        virtual void height(size_t height) {_height = height;}
        virtual bool isFocused() const {return _isFocused;}
        virtual void isFocused(bool focus) {_isFocused = focus;}

        virtual Align horizontalAlign() const {return _horizontalAlign;}
        virtual void horizontalAlign(Align horizontalAlign) {_horizontalAlign = horizontalAlign; updatePos();}
        virtual Align verticalAlign() const {return _verticalAlign;}
        virtual void verticalAlign(Align verticalAlign) {_verticalAlign = verticalAlign; updatePos();}

        virtual Repeat horizontalRepeat() const {return _horizontalRepeat;}
        virtual void horizontalRepeat(Repeat horizontalRepeat) {_horizontalRepeat = horizontalRepeat;}
        virtual Repeat verticalRepeat() const {return _verticalRepeat;}
        virtual void verticalRepeat(Repeat verticalRepeat) {_verticalRepeat = verticalRepeat;}

        virtual Vector2d relativePos() const {return _relativePos;}
        virtual void relativePos(Vector2d relativePos) {_relativePos = relativePos; updatePos();}
    protected:
        Vector2d _relativePos;

        Rect _srcRect;
        std::function<void(VisualElement*, const Mouse&)> _onClick;
        std::function<void(VisualElement*, const Mouse&)> _onHoverStart;
        std::function<void(VisualElement*, const Mouse&)> _onHoverEnd;

        bool _isAbove = false;

        size_t _width = 0u;
        size_t _height = 0u;

        Align _horizontalAlign = Align::START;
        Align _verticalAlign = Align::START;

        Repeat _horizontalRepeat = Repeat::NONE;
        Repeat _verticalRepeat = Repeat::NONE;

        bool _isFocused = false;
    };
}