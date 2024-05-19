#pragma once

#include "visualElement.h"

// namespace engix
// {
//     //Just a slider implementation for interfaces.
//     class Slider : public VisualElement
//     {
//     public:
//         Slider();
//         Slider(const Texture* line, Vector2d position, Vector2i padding, unsigned width, unsigned height, const Texture* point, Vector2i pointPadding, unsigned pointWidth, unsigned pointHeight);
//         virtual void update(const Mouse& mouse) override;
//         virtual void render() const override;
//     public:
//         double value() const {return _value;}
//         void value(double value) {_value = value;}

//         const VisualElement& point() const {return _point;}
//     protected:
//         VisualElement _point;
//         double _value;
//     };
// }