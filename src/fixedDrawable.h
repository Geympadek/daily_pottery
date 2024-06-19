#pragma once

#include "texture.h"
#include "input.h"

using std::shared_ptr;

namespace engix
{ 
    class FixedDrawable
    {
    public:
        FixedDrawable() {};
        FixedDrawable(shared_ptr<Texture> texture) : _enable(true), _texture(std::move(texture)) {}
        virtual ~FixedDrawable() {}

        virtual void render() const {_texture->render(_position, _scale, _rotation, _position, _flip, _scaling);}
        virtual void update(Input& input)
        {
        }
    public:
        virtual bool enable() const {return _enable;}
        virtual void enable(bool enable) {_enable = enable;}
    
        virtual Vector2d position() const {return _position;}
        virtual void position(Vector2d position) {_position = position;}

        virtual double scale() const {return _scale;}
        virtual void scale(double scale) {_scale = scale;}

        virtual Rotation rotation() const {return _rotation;}
        virtual void rotation(Rotation rotation) {_rotation = rotation;}

        virtual Texture::Flip flip() const {return _flip;}
        virtual void flip(Texture::Flip flip) {_flip = flip;}

        virtual Texture::Scaling scaling() const {return _scaling;}
        virtual void scaling(Texture::Scaling scaling) {_scaling = scaling;}
    protected:
        bool _enable = false;
    
        Vector2d _position;
        double _scale = 1.0;

        Rotation _rotation;

        Texture::Flip _flip = Texture::Flip::NONE;
        Texture::Scaling _scaling = Texture::Scaling::NONE;
        shared_ptr<Texture> _texture = nullptr;
    };
}