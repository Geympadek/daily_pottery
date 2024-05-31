#pragma once

#include "font.h"
#include "boxTemplate.h"
#include "visualelement.h"

namespace engix
{
    enum class Size {FIXED, AUTO};
    
    class TextBox : public VisualElement
    {
    public:
        TextBox() {}
        TextBox(std::shared_ptr<BoxTemplate> background);
        TextBox(std::shared_ptr<BoxTemplate> background, int width, int height);

        virtual void update(const Mouse& mouse) override;
        virtual void render() const override;
    public:
        virtual void width(int width) override 
        {
            VisualElement::width(width);
            _horizontalSize = Size::FIXED;
            _updateBackground = true;
        }
        virtual void height(int height) override 
        {
            VisualElement::height(height);
            _verticalSize = Size::FIXED;
            _updateBackground = true;
        }

        virtual const std::shared_ptr<Font>& font() const {return _font;}
        virtual void font(std::shared_ptr<Font> font) {_font = font;}

        virtual const std::u16string& text() const {return _text;}
        virtual void text(std::u16string text) 
        {
            _text = std::move(text);
            _updateSize = true;
        }

        virtual Color textColor() const {return _textColor;}
        virtual void textColor(Color color) {_textColor = color;}

        virtual Size horizontalSize() const {return _horizontalSize;}
        virtual void horizontalSize(Size horizontalSize) 
        {
            _horizontalSize = horizontalSize;
            _updateSize = true;
        }

        virtual Size verticalSize() const {return _verticalSize;}
        virtual void verticalSize(Size verticalSize) 
        {
            _verticalSize = verticalSize;
            _updateSize = true;
        }
    protected:
        std::u16string _text;
        Color _textColor = Color::WHITE;
        std::shared_ptr<Font> _font;

        bool _updateSize = true;
        bool _updateBackground = true;

        Size _horizontalSize = Size::AUTO;
        Size _verticalSize = Size::AUTO;

        shared_ptr<BoxTemplate> _background;
    };
}