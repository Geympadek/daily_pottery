#pragma once

#include "texture.h"
#include "timer.h"
#include "jsonval.h"

namespace engix
{
    class Animation : public Resource
    {
    public:
        Animation() noexcept {}
        template <class... Args>
        Animation(Args&&... params) : Animation(load(std::forward<Args>(params)...)) {}
        ~Animation();

        Animation(const Animation& animation) = default;
        Animation(Animation& animation) : Animation(static_cast<const Animation&>(animation)) {}

        Animation(Animation&& animation) = default;

        Animation& operator=(const Animation& animation) = default;
        Animation& operator=(Animation&& animation) = default;

        static Animation load(const assets::Path& path);
        void free() noexcept;
        
        void render(long double millis, Vector2i position, double scale = 1.0, Rotation rotation = {}, Vector2i center = {}, Texture::Flip flip = Texture::Flip::NONE) const;
    public:
        int width() const {return _width;}
        int height() const {return _height;}
        int numberOfFrames() const {return _numberOfFrames;}
        long double duration() const {return _duration;}
    private:
        void loadDelays(const json::Value& json);
    private:
        Texture _sheet;
        int _numberOfFrames = 0;
        std::vector<long double> _delays;
        long double _duration = 0;

        int _width = 0;
        int _height = 0;
    };
}