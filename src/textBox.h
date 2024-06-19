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

        virtual void update(Input& input) override;
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

        virtual const Align horizontalTextAlign() const {return _horizontalTextAlign;}
        virtual void horizontalTextAlign(Align align) {_horizontalTextAlign = align;}
        virtual const Align verticalTextAlign() const {return _verticalTextAlign;}
        virtual void verticalTextAlign(Align align) {_verticalTextAlign = align;}

        virtual Color textColor() const {return _textColor;}
        virtual void textColor(Color color) {_textColor = color;}
        virtual Color selectedTextColor() const {return _selectedTextColor;}
        virtual void selectedTextColor(Color color) {_selectedTextColor = color;}
        virtual Color selectionColor() const {return _selectionColor;}
        virtual void selectionColor(Color color) {_selectionColor = color;}

        virtual Size horizontalSize() const {return _horizontalSize;}
        virtual void horizontalSize(Size horizontalSize) 
        {
            _horizontalSize = horizontalSize;
            _updateSize = true;
        }

        virtual Size verticalSize() const noexcept {return _verticalSize;}
        virtual void verticalSize(Size verticalSize) noexcept
        {
            _verticalSize = verticalSize;
            _updateSize = true;
        }
        
        virtual int minWidth() const noexcept {return _minWidth;}
        virtual void minWidth(int minWidth) noexcept 
        {
            _minWidth = minWidth;
            _updateSize = true;
        }
        
        virtual int maxWidth() const noexcept {return _maxWidth;}
        virtual void maxWidth(int maxWidth) noexcept 
        {
            _maxWidth = maxWidth;
            _updateSize = true;
        }
        virtual int minHeight() const noexcept {return _minHeight;}
        virtual void minHeight(int minHeight) noexcept 
        {
            _minHeight = minHeight;
            _updateSize = true;
        }
        
        virtual int maxHeight() const noexcept {return _maxHeight;}
        virtual void maxHeight(int maxHeight) noexcept 
        {
            _maxHeight = maxHeight;
            _updateSize = true;
        }
    protected:
        void updateSize();
    protected:
        std::u16string _text;

        Vector2i _textPosition;
        
        Align _horizontalTextAlign = Align::START;
        Align _verticalTextAlign = Align::START;

        Color _textColor = Color::WHITE;
        Color _selectedTextColor = Color::BLUE;
        Color _selectionColor = Color::CYAN;

        std::shared_ptr<Font> _font;

        bool _updateSize = true;
        bool _updateBackground = true;

        Size _horizontalSize = Size::AUTO;
        Size _verticalSize = Size::AUTO;

        int _minWidth = 0;
        int _minHeight = 0;
        int _maxWidth = std::numeric_limits<int>::max();
        int _maxHeight = std::numeric_limits<int>::max();

        shared_ptr<BoxTemplate> _background;
    };
}