#pragma once

#include "fixedDrawable.h"
#include <iostream>
#include <SDL.h>

namespace engix
{
    //Cursor incapsulation
    class Mouse : public FixedDrawable 
    {
    public:
        enum State {NONE, LEFT = 1, MIDDLE = 2, RIGHT = 4};
    public:
        Mouse() {}
        Mouse(std::shared_ptr<Texture> texture);
        void update();
        virtual void render() const override;
    public:
        bool isShown() const {return _isShown;}
        void isShown(bool isShown) {_isShown = isShown;}
        int state() const {return _state;}
        void state(int state) {_state = state;}
        Vector2i movement() const {return _movement;}
        void movement(Vector2i movement) {_movement = movement;}
    private:
        Vector2i _movement;
        bool _isShown = true;
        int _state = State::NONE;
    };
}