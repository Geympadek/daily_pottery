#pragma once

#include "content.h"

// namespace engix
// {
//     class ContentWrap : public VisualElement
//     {
//     public:
//         ContentWrap() {}
//         ContentWrap(const Texture* texture, Vector2d position, Vector2d padding, size_t width, size_t height, Content content, Rect srcRect = gScreen, Align horizontalAlign = Align::START, Align verticalAlign = Align::START); 
//         virtual void update(const Mouse& mouse);
//     protected:
//         void updateContent();
//     public:
//         virtual void position(Vector2d position) override {VisualElement::position(position); updateContent();}

//         virtual Vector2i padding() const {return _padding;}
//         virtual void padding(Vector2i padding) {_padding = padding; updateContent();}
//     protected:
//         bool _isFocused = false;
//         Vector2i _padding;

//         Content _content;
//     };
// }