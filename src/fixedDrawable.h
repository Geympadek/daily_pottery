#pragma once

#include "texture.h"

namespace engix
{ 
    extern double pixelScale;

    //Object that isn't affected by camera's position
    class FixedDrawable
    {
    public:
        FixedDrawable() {};
        FixedDrawable(std::shared_ptr<Texture> texture, Vector2d position = {}, Texture::Scaling scaling = Texture::Scaling::NONE, Texture::Flip flip = Texture::Flip::NONE);
        virtual ~FixedDrawable() {}

        virtual void render() const;
    public:
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
        Vector2d _position;
        double _scale = 1.0;

        Rotation _rotation;

        Texture::Flip _flip = Texture::Flip::NONE;
        Texture::Scaling _scaling = Texture::Scaling::STRETCH;
        std::shared_ptr<Texture> _texture = nullptr;
    };
}