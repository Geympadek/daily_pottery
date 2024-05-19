#pragma once

#include "visualelement.h"
#include "font.h"

// namespace engix
// {
//     class Content : public VisualElement
//     {
//     public:
//         Content() {}
//         Content(const Texture* texture, Vector2d position, Rect srcRect = gScreen, Align horizontalAlign = Align::START, Align verticalAlign = Align::START);
//         Content(std::u16string text, Color color, const Font& font, Vector2d position, Rect srcRect = gScreen, Align horizontalAlign = Align::START, Align verticalAlign = Align::START);
    
//         virtual void render() const override;
//     public:
//         virtual const std::u16string& text() const {return _text;}
//         virtual void text(std::u16string text) {_text = std::move(text);}
//         virtual Color color() const {return _color;}
//         virtual void color(Color color) {_color = color;}
//         virtual const Font& font() const {return *_font;}
//         virtual void font(Font& font) {_font = &font;}
//     protected:
//         std::u16string _text;
//         Color _color = Color::BLACK;
//         const Font* _font = nullptr;
//     };
// }