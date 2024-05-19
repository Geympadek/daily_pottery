// #include "slider.h"

// engix::Slider::Slider() : VisualElement(), _point(), _value(0)
// {
// }

// engix::Slider::Slider(const Texture* line, Vector2d position, Vector2i padding, unsigned width, unsigned height, const Texture* point, Vector2i pointPadding, unsigned pointWidth, unsigned pointHeight)
//  : VisualElement(line, position, padding, width, height), _point(point, position + padding, pointPadding, pointWidth, pointHeight), _value(0)
// {
// }

// void engix::Slider::update(const Mouse &mouse)
// {
//     _point.update(mouse);
//     VisualElement::update(mouse);
//     if (_isFocused || _point.isFocused())
//     {
//         auto& pointPos = _point.position();
//         pointPos = mouse.position() - Vector2i{_point.width() / 2, 0};

//         auto start = _position.x + _padding.x;
//         auto end = _position.x + _width - _point.width();
//         if (pointPos.x < start)
//         {
//             pointPos.x = start;
//         }
//         else if (pointPos.x > end)
//         {
//             pointPos.x = end;
//         }

//         auto length = end - start;
//         auto pointShift = pointPos.x - _position.x;

//         _value = static_cast<double>(pointShift) / length;
//         std::cout << _value << '\n';
//         pointPos.y = _position.y + _padding.y;
//     }
// }

// void engix::Slider::render() const
// {
//     VisualElement::render();//rendering the line
//     _point.render();
// }