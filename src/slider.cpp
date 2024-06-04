#include "slider.h"
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
//     if (_isClicked || _point.isFocused())
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

engix::Slider::Slider(Direction orientation, std::shared_ptr<Texture> line, std::shared_ptr<Texture> point, Vector2i pointShift, int length)
 : _orientation(orientation), VisualElement(std::move(line)), _point(std::move(point)), _pointShift(pointShift), _length(length)
{
    _point.relativePos(pointShift);
}

void engix::Slider::update(Input& input)
{
    VisualElement::update(input);

    if (_isClicked)
    {
        int start;
        int end = _length;
        
        Vector2i pointPos = _point.position();

        int mousePos;
        if (_orientation == Direction::HORIZONTAL)
        {
            start = static_cast<int>(_position.x) + _pointShift.x;
            end += start;

            mousePos = static_cast<int>(input.cursor.position.x) - _point.width() / 2;
            mousePos = std::clamp(mousePos, start, end);
            pointPos.x = mousePos;
        }
        else
        {
            start = static_cast<int>(_position.y) + _pointShift.y;
            end += start;

            mousePos = static_cast<int>(input.cursor.position.y) - _point.height() / 2;
            mousePos = std::clamp(mousePos, start, end);
            pointPos.y = mousePos;
        }
        
        _point.position(pointPos);
        _value = static_cast<double>(mousePos - start) / _length;
    }
}

void engix::Slider::render() const
{
    VisualElement::render();
    _point.render();
}

void engix::Slider::updatePos()
{
    VisualElement::updatePos();

    _point.srcRect({_position, _width, _height});
}
