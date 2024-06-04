#pragma once

#include "mouse.h"
#include <functional>

namespace engix
{
    enum class Align {START, CENTER, END};
    enum class Repeat {NONE, REPEAT};
    //Any interface element
    class VisualElement : public FixedDrawable
    {
    public:
        VisualElement() {}
        VisualElement(std::shared_ptr<Texture> texture);
        VisualElement(std::shared_ptr<Texture> texture, int width, int height);
        
        virtual void update(Input& input);
        virtual void render() const override;
    protected:
        virtual void updatePos();
        virtual void renderContent() const;
    public:
        virtual Rect srcRect() const {return _srcRect;}
        virtual void srcRect(Rect rect) 
        {
            _srcRect = rect; 
            updatePos();
        }

        Rect rect() const {return Rect(_position, _width, _height);}

        virtual void onClick(std::function<void(VisualElement*, Input&)> onClick) {_onClick = onClick;}
        virtual void onHoverStart(std::function<void(VisualElement*, Input&)> onHoverStart) {_onHoverStart = onHoverStart;}
        virtual void onHoverEnd(std::function<void(VisualElement*, Input&)> onHoverEnd) {_onHoverEnd = onHoverEnd;}

        virtual int width() const {return _width;}
        virtual void width(int width) {_width = width;}
        virtual int height() const {return _height;}
        virtual void height(int height) {_height = height;}

        virtual int paddingLeft() const {return _paddingLeft;}
        virtual void paddingLeft(int paddingLeft) {_paddingLeft = paddingLeft;}
        virtual int paddingUp() const {return _paddingUp;}
        virtual void paddingUp(int paddingUp) {_paddingUp = paddingUp;}
        virtual int paddingRight() const {return _paddingRight;}
        virtual void paddingRight(int paddingRight) {_paddingRight = paddingRight;}
        virtual int paddingDown() const {return _paddingDown;}
        virtual void paddingDown(int paddingDown) {_paddingDown = paddingDown;}

        virtual void padding(int left, int up, int right, int down) 
        {
            _paddingLeft = left;
            _paddingUp = up;
            _paddingRight = right;
            _paddingDown = down;
        }

        virtual bool isClicked() const {return _isClicked;}
        virtual void isClicked(bool isClicked) {_isClicked = isClicked;}

        virtual bool isActive() const {return _isActive;}
        virtual void isActive(bool isActive) {_isActive = isActive;}

        virtual Align horizontalAlign() const {return _horizontalAlign;}
        virtual void horizontalAlign(Align horizontalAlign) 
        {
            _horizontalAlign = horizontalAlign; 
            updatePos();
        }
        virtual Align verticalAlign() const {return _verticalAlign;}
        virtual void verticalAlign(Align verticalAlign) 
        {
            _verticalAlign = verticalAlign;
            updatePos();
        }

        virtual Vector2d relativePos() const {return _relativePos;}
        virtual void relativePos(Vector2d relativePos) 
        {
            _relativePos = relativePos; 
            updatePos();
        }

        virtual std::vector<VisualElement*>& content() 
        {
            _updatePos = true;
            return _content;
        }
        virtual const std::vector<VisualElement*>& content() const {return _content;}
    protected:
        int _width = 0;
        int _height = 0;

        int _paddingLeft = 0, _paddingUp = 0, _paddingRight = 0, _paddingDown = 0;

        std::vector<VisualElement*> _content;

        Vector2d _relativePos;

        Rect _srcRect = gScreen;

        std::function<void(VisualElement*, Input&)> _onClick;
        std::function<void(VisualElement*, Input&)> _onHoverStart;
        std::function<void(VisualElement*, Input&)> _onHoverEnd;

        bool _updatePos = true;

        bool _isAbove = false;//Is cursor above element?
        bool _isActive = false;//Was this element clicked the last time or sth else?
        bool _isClicked = false;//Is mouse pressed?

        Align _horizontalAlign = Align::START;
        Align _verticalAlign = Align::START;
    };
}