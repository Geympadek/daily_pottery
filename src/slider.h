#pragma once

#include "visualElement.h"
#include "boxTemplate.h"

namespace engix
{
    class Slider : public VisualElement
    {
    public:
        Slider() {}
        Slider(Direction orientation, const BoxTemplate& line, int width, int height, std::shared_ptr<Texture> point, Vector2i pointShift)
         : Slider(orientation, line, width, height, point, pointShift, (orientation == Direction::HORIZONTAL ? width - pointShift.x * 2 - point->width() : height - pointShift.y * 2 - point->height())) {}

        Slider(Direction orientation, const BoxTemplate& line, int width, int height, std::shared_ptr<Texture> point, Vector2i pointShift, int moveableLength)
         : Slider(orientation, std::make_shared<Texture>(line.genTexture(width, height)), point, pointShift, moveableLength) {}

        Slider(Direction orientation, std::shared_ptr<Texture> line, std::shared_ptr<Texture> point, Vector2i pointShift, int length);
        
        virtual void update(Input& input) override;
        virtual void render() const override;
    protected:
        virtual void updatePos() override;
    public:
        double value() const {return _value;}
        Direction orientation() const {return _orientation;}    
    protected:
        Direction _orientation;

        VisualElement _point;
        Vector2i _pointShift;

        int _length;
        double _value;
    };
}