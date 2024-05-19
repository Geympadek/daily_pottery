// #include "content.h"

// engix::Content::Content(const Texture *texture, Vector2d position, Rect srcRect, Align horizontalAlign, Align verticalAlign) 
// : VisualElement(texture, position, texture->width(), texture->height(), srcRect, horizontalAlign, verticalAlign)
// {
// }

// engix::Content::Content(std::u16string text, Color color, const Font& font, Vector2d position, Rect srcRect, Align horizontalAlign, Align verticalAlign)
// {
//     _position = position;
//     _text = std::move(text);
//     _font = &font;
//     auto size = font.findSize(text);
//     _width = size.x;
//     _height = size.y;
//     _horizontalAlign = horizontalAlign;
//     _verticalAlign = verticalAlign;
//     _srcRect = srcRect;
// }

// void engix::Content::render() const
// {
//     if (_texture != nullptr)
//     {
//         VisualElement::render();
//     }
//     if (_font != nullptr)
//     {
//         _font->render(_text, _calcPosition, _color);
//     }
// }
